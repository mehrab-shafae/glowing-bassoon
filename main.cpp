/**=============================================================================*
        Copyright © 2023, 2024, 2025, 2026, Marboris Foundation.
        Copyright © 2026, Mrb.
        See [COPYING](COPYING) for more information.
*=============================================================================**/
//*----------------------------------------------------------------------------**
#include "lib/core.h"
//*----------------------------------------------------------------------------**
// #include "test/tpool.cc"
#include "test/f.cc"

//*----------------------------------------------------------------------------**
#define LOG_FILE_NAME "logs/core.log"
//*----------------------------------------------------------------------------**
string get_timestamp()
{
    const auto now        = std::chrono::system_clock::now();
    const auto time_point = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_point), "%H:%M-%d.%m.%y+%Z");
    return ss.str();
}
//*----------------------------------------------------------------------------**
void write_to_console(string_view level, string_view msg)
{
        const let timestamp = get_timestamp();
        std::cout << "[" << *timestamp << "] [" << level << "]: " << msg
                  << std::endl;
}

bool write_to_file(string_view level, string_view msg)
{
    
        const let timestamp = get_timestamp();
        const string log_entry = "[" + *timestamp + "] [" +
                                      string(level) + "] " +
                                      string(msg) + "\n";

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

void log(string_view msg)
{
    write_to_file("INF", msg);
    write_to_console("INF", msg);
}

void error(string_view msg)
{
    write_to_file("ERR", msg);
    write_to_console("ERR", msg);
}

void out(string_view level, string_view msg)
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
    f();

    return 1;
}
//*----------------------------------------------------------------------------**
//*----------------------------------------------------------------------------**