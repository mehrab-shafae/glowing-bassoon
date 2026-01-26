#include <coroutine>
#include <chrono>
#include <iostream>
#include <queue>
#include <vector>
#include <thread>

using namespace std::chrono_literals;

// -------------------------------------------
// Scheduler Singleton با lazy-start
// -------------------------------------------
struct Scheduler {
    struct Timer {
        std::chrono::steady_clock::time_point wakeup;
        std::coroutine_handle<> handle;
        bool operator>(const Timer& other) const { return wakeup > other.wakeup; }
    };

    std::priority_queue<Timer, std::vector<Timer>, std::greater<>> timers;
    int active_tasks = 0;
    bool loop_running = false;

    static Scheduler& instance() {
        static Scheduler sched;
        return sched;
    }

    void add_task() {
        active_tasks++;
        // اگر loop هنوز اجرا نشده است، آن را خودکار اجرا کن
        if (!loop_running) {
            loop_running = true;
            std::thread([this]{ this->loop(); }).detach();
        }
    }

    void task_done() { active_tasks--; }

    void add_timer(std::coroutine_handle<> h, std::chrono::milliseconds duration) {
        timers.push({std::chrono::steady_clock::now() + duration, h});
    }

private:
    void loop() {
        while (active_tasks > 0 || !timers.empty()) {
            if (timers.empty()) continue;

            auto now = std::chrono::steady_clock::now();
            auto next = timers.top();
            if (next.wakeup <= now) {
                timers.pop();
                next.handle.resume();
            } else {
                std::this_thread::sleep_for(next.wakeup - now);
            }
        }
    }
};

// -------------------------------------------
// Awaitable Sleep
// -------------------------------------------
struct Sleep {
    std::chrono::milliseconds duration;

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
            Scheduler::instance().add_task(); // ثبت خودکار
            return {}; 
        }

        struct final_awaiter {
            bool await_ready() noexcept { return false; }
            void await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                Scheduler::instance().task_done(); // خودکار وقتی پایان یافت
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
// Main بسیار ساده
// -------------------------------------------
int main() {
    async_task(1, 1000);
    async_task(2, 2000);
    async_task(3, 1500);

    // هیچ چیز دیگری لازم نیست، برنامه خودش تا پایان همه taskها اجرا می‌شود
    std::this_thread::sleep_for(2500ms); // فقط برای مثال، در نسخه کامل lazy می‌توان از condition_variable استفاده کرد
}
