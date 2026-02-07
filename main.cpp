#include <future>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <string>
#include <string_view>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <mutex>
#include <format>
// --------------------------------------------------------------------
#define LOG_FILE_NAME "logs/core.log"
#define LOG_TIME_Z "%H:%M-%d.%m.%y+%Z"

#define CERR_CKEY "ERR"
#define COUT_CKEY "INF"
// --------------------------------------------------------------------

// --------------------------------------------------------------------
std::string get_timestamp()
{
    const auto time_point = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_point), LOG_TIME_Z);
    return ss.str();
}
// --------------------------------------------------------------------

// --------------------------------------------------------------------
namespace console::in {

void console(std::string& val, const std::string_view msg)
{
        std::cin >> val;
}

} // namespace in
// --------------------------------------------------------------------
namespace console::out {

void console(const std::string_view level, const std::string_view msg)
{
        const auto timez = std::format("[{}] [{}]: ",
                                        get_timestamp(),
                                        level);

        if(level == CERR_CKEY) std::cerr << timez << msg << "\n";
        std::cout << timez << msg << "\n";
}

bool file(const std::string_view level, const std::string_view msg)
{
        std::ofstream log_file{LOG_FILE_NAME, std::ios::app};
        if(log_file.is_open())
        {
            log_file << std::format("[{}] [{}] {}\n",
                                    get_timestamp(),
                                    level,
                                    msg);
            return true;
        }
        return false;
}

} // namespace out
// --------------------------------------------------------------------
namespace console {

void log(const std::string_view msg)
{
    console::out::file(COUT_CKEY, msg);
    console::out::console(COUT_CKEY, msg);
}

void error(const std::string_view msg)
{
    console::out::file(CERR_CKEY, msg);
    console::out::console(CERR_CKEY, msg);
}

// void out(const std::string_view level, const std::string_view msg)
// {
//     console::out::file(level, msg);
//     console::out::console(level, msg);
// }

}  // namespace console
// --------------------------------------------------------------------

// --------------------------------------------------------------------
int main(const int argc, const char* argv[]) {
    console::log("Hi");

    // std::string valu;
    // console::in::console(valu, "Hi");
    // console::log(valu);

    return 0;
}
// --------------------------------------------------------------------