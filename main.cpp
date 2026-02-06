#include "core.hpp"
// --------------------------------------------------------------------
#define LOG_FILE_NAME "logs/core.log"
#define LOG_TIME_Z "%H:%M-%d.%m.%y+%Z"
// --------------------------------------------------------------------

// --------------------------------------------------------------------
std::string get_timestamp()
{
    const auto now        = std::chrono::system_clock::now();
    const auto time_point = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_point), LOG_TIME_Z);
    return ss.str();
}
// --------------------------------------------------------------------

// --------------------------------------------------------------------
namespace out {

void console(std::string_view level, std::string_view msg)
{
        const auto timestamp = get_timestamp();
        std::cout << "[" << std::string(timestamp) << "] [" << level << "]: " << msg
                  << std::endl;
}

bool file(std::string_view level, std::string_view msg)
{
        const auto timestamp = get_timestamp();
        const auto log_entry = "[" + std::string(timestamp) + "] [" +
                                      std::string(level) + "] " +
                                      std::string(msg) + "\n";

        std::ofstream log_file{LOG_FILE_NAME, std::ios::app};
        if(log_file.is_open())
        {
            log_file << log_entry;
            return true;
        }
        return false;
}

} // namespace out
// --------------------------------------------------------------------
namespace console {

void log(std::string_view msg)
{
    out::file("INF", msg);
    out::console("INF", msg);
}

void error(std::string_view msg)
{
    out::file("ERR", msg);
    out::console("ERR", msg);
}

void out(std::string_view level, std::string_view msg)
{
    out::file(level, msg);
    out::console(level, msg);
}

}  // namespace console
// --------------------------------------------------------------------

// --------------------------------------------------------------------
int main() {
    console::log("Hi");

    return 0;
}
// --------------------------------------------------------------------