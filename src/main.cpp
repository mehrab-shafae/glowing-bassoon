#include "core.hpp"
#include "console.hh"
#include "db.hh"
#include "proc.hh"

#define highclock_start const auto start_time = std::chrono::high_resolution_clock::now()
//--------------------------------------------
static highclock_start;
// --------------------------------------------------------------------
static void user_time() noexcept {
    echo("\n");
    const auto end_time = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end_time - start_time;

    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);

    const auto user_time = static_cast<double>(usage.ru_utime.tv_sec) +
                            static_cast<double>(usage.ru_utime.tv_usec) / 1e6;

    if (elapsed.count() <= 2.0) {
        std::cout << std::fixed << std::setprecision(6);
        echo("core time: " << user_time << "s user total, ");
    }
    echo("uptime: " << elapsed.count() << "s");
}
//--------------------------------------------
class InitClass final {
public:
    InitClass(const InitClass &) = delete;
    InitClass &operator=(const InitClass &) = delete;

    InitClass() noexcept {
        atexit(user_time);
    }
};
static const InitClass init_class;

inline void $head(int argc, char *argv[]) __attribute__((used));
inline void $run() __attribute__((used));
inline void $init() __attribute__((constructor, used));
inline void $end() __attribute__((destructor, used));
// --------------------------------------------------------------------
int main(const int argc, const char* argv[]) {
    try
    {
        console::log("Hi");

        db::unit0::join("mehrab");

        console::log("db joined");

        echo(it());
        // std::string valu;
        // console::in::console(valu, "Hi");
        // console::log(valu);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
// --------------------------------------------------------------------