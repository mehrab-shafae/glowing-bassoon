//*----------------------------------------------------------------------------**
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
//*----------------------------------------------------------------------------**
#define LOG_FILE_NAME "logs/core.log"
//*----------------------------------------------------------------------------**
std::string get_timestamp()
{
    const auto now        = std::chrono::system_clock::now();
    const auto time_point = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_point), "%H:%M-%d.%m.%y+%Z");
    return ss.str();
}
//*----------------------------------------------------------------------------**
void write_to_console(std::string_view level, std::string_view msg)
{
        const std::string_view timestamp = get_timestamp();
        std::cout << "[" << std::string(timestamp) << "] [" << level << "]: " << msg
                  << std::endl;
}

bool write_to_file(std::string_view level, std::string_view msg)
{
    
        const std::string_view timestamp = get_timestamp();
        const std::string log_entry = "[" + std::string(timestamp) + "] [" +
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
//*----------------------------------------------------------------------------**
namespace console {

void log(std::string_view msg)
{
    write_to_file("INF", msg);
    write_to_console("INF", msg);
}

void error(std::string_view msg)
{
    write_to_file("ERR", msg);
    write_to_console("ERR", msg);
}

void out(std::string_view level, std::string_view msg)
{
    write_to_file(level, msg);
    write_to_console(level, msg);
}

}  // namespace console
//*----------------------------------------------------------------------------**
int log_test() {
    console::log("Hi me");
    return 0;
    return 1;
    return 1;
}
//*----------------------------------------------------------------------------**
int main() {
    // std::cout;

    // if(log_test()){
    //     console::error("Error!");
    // }

    // console::log("tpool test");

    // if(tpool()){
    //     console::error("Error!");
    // }
    // f();
    // corul();

    return 1;
}
//*----------------------------------------------------------------------------**
//*----------------------------------------------------------------------------**