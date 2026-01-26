// simple_pool_with_operator.cpp
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <optional>
#include <atomic>
#include <chrono>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency() * 2)
        : stop(false)
    {
        if (numThreads == 0) numThreads = 2; // حداقل 2 ترد
        workers.reserve(numThreads);
        for (size_t i = 0; i < numThreads; ++i) {
            // هر jthread حلقه پردازش را اجرا می‌کند
            workers.emplace_back(&ThreadPool::processTasks, this);
        }
    }

    ~ThreadPool() {
        // علامت خاموشی و بیدار کردن همه workerها
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();

        // پاک کردن vector باعث می‌شود destructor هر std::jthread اجرا و در صورت نیاز join شود
        workers.clear();
    }

    // ارسال تسک با syntax ساده: pool << []{ ... };
    template <typename Func>
    ThreadPool &operator<<(Func &&func) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            // بسته‌بندی callable به std::function<void()>
            tasks.emplace(std::function<void()>(std::forward<Func>(func)));
        }
        cv.notify_one();
        return *this; // اجازه chaining: pool << t1 << t2;
    }

    // اختیاری: صبر کن تا تمام تسک‌های فعلی اجرا شوند (می‌توان در مثال از آن استفاده کرد)
    void waitIdle() {
        std::unique_lock<std::mutex> lock(mtx);
        idle_cv.wait(lock, [this] { return tasks.empty(); });
    }

private:
    void processTasks() {
        while (true) {
            std::optional<std::function<void()>> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return stop || !tasks.empty(); });

                // اگر خاموش شدیم و صف خالی است، خارج شو
                if (stop && tasks.empty()) return;

                // برداشتن تسک از صف
                task = std::move(tasks.front());
                tasks.pop();

                // اگر صف خالی شد، به waitIdle اطلاع بده
                if (tasks.empty()) idle_cv.notify_all();
            }

            // اجرای تسک با محافظت در برابر استثنا
            try {
                if (task && *task) {
                    (*task)();
                }
            } catch (const std::exception &e) {
                std::cerr << "[ThreadPool] task exception: " << e.what() << '\n';
            } catch (...) {
                std::cerr << "[ThreadPool] task unknown exception\n";
            }
        }
    }

    std::vector<std::jthread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::condition_variable idle_cv; // برای تابع waitIdle
    std::atomic<bool> stop;
};

void exampleTask(int id) {
    std::cout << "task " << id << " start (thread " << std::this_thread::get_id() << ")\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout << "task " << id << " done\n";
}

int f() {
    ThreadPool pool(4); // 4 worker؛ یا بدون پارامتر برای خودکار

    // ارسال تسک‌ها با اپراتور <<
    for (int i = 0; i < 8; ++i) {
        pool << [i]{ exampleTask(i); };
    }

    // اگر بخواهی صبر کنی تا صف خالی شود:
    pool.waitIdle();

    std::cout << "all tasks finished (main will exit, pool destructor will join workers)\n";
    return 0;
}
