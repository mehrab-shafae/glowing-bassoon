// pool_with_per_worker_queues.cpp
// C++20 (uses std::jthread / stop_token)
// طراحی: ثابت بودن نخ‌ها، dispatcher + per-worker queues, cooperative pause/resume.

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <chrono>
#include <memory>
#include <optional>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads)
        : stop_flag(false), next_worker(0)
    {
        if (numThreads == 0) numThreads = 1;
        workers.reserve(numThreads);

        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(std::make_unique<Worker>(i));
            // start jthread and give it the workerLoop for this id
            workers.back()->thread = std::jthread([this, i](std::stop_token st){
                this->workerLoop(i, st);
            });
        }
    }

    ~ThreadPool() {
        stop_flag.store(true, std::memory_order_relaxed);
        // notify all workers so they can exit
        for (auto &wptr : workers) {
            {
                std::lock_guard lk(wptr->mtx);
            }
            wptr->cv.notify_all();
        }
        // also wake workers waiting on global queue
        {
            std::lock_guard lk(dispatcher_mtx);
        }
        for (auto &wptr : workers) wptr->cv.notify_all();

        // jthread will join in its destructor automatically
    }

    // submit: returns future<R>
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using R = std::invoke_result_t<F, Args...>;
        auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<R()>>(std::move(bound));
        std::future<R> fut = task_ptr->get_future();

        auto wrapper = [task_ptr]() {
            try {
                (*task_ptr)();
            } catch (...) {
                // packaged_task already stores exception in future
                throw;
            }
        };

        // try to dispatch directly to a free (not active & not paused) worker
        if (!try_dispatch_to_idle_worker(wrapper)) {
            // fallback: push to global backlog
            {
                std::lock_guard lk(dispatcher_mtx);
                global_queue.push(std::move(wrapper));
            }
            // wake up one (or all) workers to try to steal from global queue
            notifyOneWorker();
        }

        return fut;
    }

    // fire-and-forget convenience
    template <typename Func>
    ThreadPool &operator<<(Func &&func) {
        submit(std::forward<Func>(func));
        return *this;
    }

    // cooperative pause/resume called from inside a task (on the thread executing it)
    void pauseCurrentWorker() {
        auto id = current_worker_id();
        if (id==npos) return;
        workers[id]->paused.store(true, std::memory_order_relaxed);
    }

    void resumeCurrentWorker() {
        auto id = current_worker_id();
        if (id==npos) return;
        workers[id]->paused.store(false, std::memory_order_relaxed);
        // when resumed and thread is idle it will pick global tasks automatically.
    }

    // wait until all queues empty and no active tasks
    void waitIdle() {
        std::unique_lock lk(idle_mtx);
        idle_cv.wait(lk, [this]{
            if (stop_flag.load()) return true;
            // check local queues
            for (auto &w : workers) {
                std::lock_guard lk2(w->mtx);
                if (!w->q.empty()) return false;
                if (w->active.load()) return false;
            }
            // check global queue
            std::lock_guard lk3(dispatcher_mtx);
            if (!global_queue.empty()) return false;
            return true;
        });
    }

    // optional utility for debugging: number of total pending tasks
    size_t pendingTasks() const {
        size_t count = 0;
        for (auto &w : workers) {
            std::lock_guard lk(w->mtx);
            count += w->q.size();
        }
        std::lock_guard lk(dispatcher_mtx);
        count += global_queue.size();
        return count;
    }

private:
    static constexpr size_t npos = static_cast<size_t>(-1);

    struct Worker {
        Worker(size_t id_) : id(id_), thread(), active(false), paused(false) {}
        size_t id;
        std::jthread thread;                 // actual thread (default constructed then assigned)
        std::mutex mtx;                      // guards q
        std::condition_variable cv;
        std::queue<std::function<void()>> q;
        std::atomic<bool> active;            // executing a task right now
        std::atomic<bool> paused;            // cooperatively paused by current task
    };

    // per-worker vector
    std::vector<std::unique_ptr<Worker>> workers;

    // global backlog queue (fallback when no idle worker)
    mutable std::mutex dispatcher_mtx;
    std::queue<std::function<void()>> global_queue;

    // stop flag
    std::atomic<bool> stop_flag;

    // round-robin hint
    std::atomic<size_t> next_worker;

    // idle wait notification
    mutable std::mutex idle_mtx;
    std::condition_variable idle_cv;

    // thread-local index of worker currently running in this thread
    static thread_local size_t tls_worker_id;

    // helper: try to dispatch directly to an idle & not-paused worker
    // bool try_dispatch_to_idle_worker(std::function<void()>& wrapper) {
    bool try_dispatch_to_idle_worker(std::function<void()> wrapper) {
        const size_t n = workers.size();
        if (n == 0) return false;
        size_t start = next_worker.fetch_add(1) % n;
        for (size_t k = 0; k < n; ++k) {
            size_t i = (start + k) % n;
            Worker *w = workers[i].get();
            // check quickly without locking if possible
            if (w->active.load(std::memory_order_relaxed)) continue;
            if (w->paused.load(std::memory_order_relaxed)) continue;
            // lock its queue and push
            {
                std::lock_guard lk(w->mtx);
                // re-check conditions under lock to avoid race
                if (w->active.load(std::memory_order_relaxed)) continue;
                if (w->paused.load(std::memory_order_relaxed)) continue;
                // w->q.push(wrapper);
                w->q.push(std::move(wrapper));
            }
            w->cv.notify_one();
            return true;
        }
        return false;
    }

    void notifyOneWorker() {
        // simple strategy: wake up worker at next_worker index
        size_t n = workers.size();
        if (n == 0) return;
        size_t idx = next_worker.fetch_add(1) % n;
        workers[idx]->cv.notify_one();
    }

    // worker loop
    void workerLoop(size_t id, std::stop_token stoken) {
        Worker *self = workers[id].get();
        while (!stoken.stop_requested() && !stop_flag.load(std::memory_order_relaxed)) {
            std::function<void()> task;

            // Wait for a task: either own queue or global queue
            {
                std::unique_lock lk(self->mtx);
                self->cv.wait(lk, [this, self]{
                    if (stop_flag.load()) return true;
                    if (!self->q.empty()) return true;
                    // check global queue (needs dispatcher_mtx)
                    std::lock_guard lk2(dispatcher_mtx);
                    return !global_queue.empty();
                });

                if (stop_flag.load()) break;

                if (!self->q.empty()) {
                    task = std::move(self->q.front());
                    self->q.pop();
                } else {
                    // try to steal from global queue
                    std::lock_guard lk2(dispatcher_mtx);
                    if (!global_queue.empty()) {
                        task = std::move(global_queue.front());
                        global_queue.pop();
                    }
                }

                if (!task) {
                    // nothing for this worker right now
                    continue;
                }
                // mark active and set tls id
                self->active.store(true, std::memory_order_relaxed);
                tls_worker_id = id;
            }

            // Execute task (exceptions are allowed to propagate into packaged_task future)
            try {
                task();
            } catch (...) {
                // swallow here (packaged_task already stores), but ensure we don't crash the worker
            }

            // finished executing
            {
                std::lock_guard lk(self->mtx);
                self->active.store(false, std::memory_order_relaxed);
                tls_worker_id = npos;
            }

            // notify waitIdle in case all done
            {
                std::lock_guard lk(idle_mtx);
                idle_cv.notify_all();
            }
        }
    }

    // returns worker id for current thread or npos if not a worker thread
    size_t current_worker_id() const {
        return tls_worker_id;
    }
};

// define static TLS
thread_local size_t ThreadPool::tls_worker_id = ThreadPool::npos;

// ----------------- Example usage -----------------

// exampleTask: demonstrates cooperative pause/resume
void exampleTask(ThreadPool &pool, int id, bool do_pause) {
    std::cout << "task " << id << " start (tid " << std::this_thread::get_id() << ")\n";

    if (do_pause) {
        // signal that this worker is going to block and should be considered paused
        // pool.pauseCurrentWorker();
        // simulate a blocking call (this is where cooperative pause is useful)
        // std::this_thread::sleep_for(std::chrono::milliseconds(400));
        // pool.resumeCurrentWorker();
    } else {
        // std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    std::cout << "task " << id << " done\n";
}

int f() {
    ThreadPool pool(1);

    // Submit 8 tasks; some will use cooperative pause to simulate long blocking operations
    for (int i = 0; i < 8; ++i) {
        bool do_pause = (i % 3 == 0); // every third task will call pause/resume
        pool << [&pool, i, do_pause] { exampleTask(pool, i, do_pause); };
    }

    // wait until everything finished
    pool.waitIdle();

    std::cout << "all tasks finished\n";
    return 0;
}
