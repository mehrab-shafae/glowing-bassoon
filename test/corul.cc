#include <coroutine>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

using namespace std::chrono_literals;

// -------------------------------------------
// Scheduler Singleton (Single-threaded, no extra threads)
// -------------------------------------------
struct Scheduler {
    struct Timer {
        std::chrono::steady_clock::time_point wakeup;
        std::coroutine_handle<> handle;

        bool operator>(const Timer& other) const { return wakeup > other.wakeup; }
    };

    std::priority_queue<Timer, std::vector<Timer>, std::greater<>> timers;
    int active_tasks = 0;

    std::mutex mtx;
    std::condition_variable cv;

    static Scheduler& instance() {
        static Scheduler sched;
        return sched;
    }

    void add_task() {
        active_tasks++;
    }

    void task_done() {
        active_tasks--;
        cv.notify_one();
    }

    void add_timer(std::coroutine_handle<> h, std::chrono::milliseconds duration) {
        timers.push({std::chrono::steady_clock::now() + duration, h});
    }

    // -------------------------------------------
    // Single-threaded loop that runs until all tasks complete
    // -------------------------------------------
    void run() {
        std::unique_lock lock(mtx);

        while (active_tasks > 0 || !timers.empty()) {
            if (timers.empty()) {
                // هیچ تایمری نیست، منتظر بمان تا task جدید اضافه شود
                cv.wait(lock, [this] { return !timers.empty() || active_tasks == 0; });
                continue;
            }

            auto now = std::chrono::steady_clock::now();
            auto next = timers.top();

            if (next.wakeup <= now) {
                timers.pop();
                lock.unlock();
                next.handle.resume();
                lock.lock();
            } else {
                cv.wait_until(lock, next.wakeup, [this, &now] {
                    now = std::chrono::steady_clock::now();
                    return !timers.empty() && timers.top().wakeup <= now;
                });
            }
        }
    }
};

// -------------------------------------------
// Awaitable Sleep
// -------------------------------------------
struct Sleep {
    const std::chrono::milliseconds duration;

    bool await_ready() const noexcept { return duration.count() <= 0; }

    void await_suspend(std::coroutine_handle<> h) const noexcept {
        Scheduler::instance().add_timer(h, duration);
    }

    void await_resume() const noexcept {}
};

// -------------------------------------------
// Task Coroutine
// -------------------------------------------
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { 
            Scheduler::instance().add_task(); 
            return {}; 
        }

        struct final_awaiter {
            bool await_ready() noexcept { return false; }
            void await_suspend(std::coroutine_handle<promise_type> /*h*/) noexcept {
                Scheduler::instance().task_done();
            }
            void await_resume() noexcept {}
        };
        final_awaiter final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// -------------------------------------------
// Async Task Example
// -------------------------------------------
Task async_task(int id, int delay_ms) {
    std::cout << "Task " << id << " started\n";
    co_await Sleep{std::chrono::milliseconds(delay_ms)};
    std::cout << "Task " << id << " finished after " << delay_ms << "ms\n";
}

// -------------------------------------------
// Main
// -------------------------------------------
int corul() {
    async_task(1, 1000);
    async_task(2, 2000);
    async_task(3, 1500);

    // این خط جایگزین تمام sleep یا thread ها می‌شود
    Scheduler::instance().run();

    std::cout << "Main finished\n";
    return 0;
}
