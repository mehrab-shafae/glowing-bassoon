#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <optional>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency() * 2) : stop(false) {
        numThreads = (numThreads != 0) ? numThreads : 2; // حداقل 2 رشته
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(&ThreadPool::processTasks, this);
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (std::jthread &worker : workers) {
            worker.join();
        }
    }

    template <typename Func>
    ThreadPool &operator<<(Func &&func) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.push(std::forward<Func>(func));
        }
        cv.notify_one();
        return *this;
    }

private:
    void processTasks() {
        while (true) {
            std::optional<std::function<void()>> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop && tasks.empty())
                    return;
                task = std::move(tasks.front());
                tasks.pop();
            }
            try {
                if (task) {
                    (*task)(); // اجرای تسک
                }
            } catch (const std::exception &e) {
                std::cerr << "Exception caught: " << e.what() << std::endl;
            }
        }
    }

    std::vector<std::jthread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;
};

void exampleTask() {
    std::cout << "Task executed by thread: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

extern int tpool() {
    std::cout << "Start tpool" << std::endl;
    ThreadPool pool; // ایجاد ThreadPool با تعداد رشته‌های خودکار

    for (size_t i = 0; i < 10; ++i) {
        pool << exampleTask; // اضافه کردن وظایف به ThreadPool
    }

    // برای اطمینان از اینکه همه وظایف اجرا شده‌اند
    // std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "End tpool" << std::endl;

    return 0;
}
