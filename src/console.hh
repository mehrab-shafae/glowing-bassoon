#pragma once
#include "core.hpp"
#include "fs.hh"
#include "time.hh"
// --------------------------------------------------------------------
#ifndef CERR_CKEY
#define CERR_CKEY "ERR"
#endif

#ifndef COUT_CKEY
#define COUT_CKEY "INF"
#endif
// --------------------------------------------------------------------
namespace console::in {

// void console(std::string& val, const std::string_view msg)
// {
//         std::cin >> val;
// }

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

} // namespace out
// --------------------------------------------------------------------
namespace console {

void log(const std::string_view msg)
{
    fs::out::console(COUT_CKEY, msg);
    console::out::console(COUT_CKEY, msg);
}

void error(const std::string_view msg)
{
    fs::out::console(CERR_CKEY, msg);
    console::out::console(CERR_CKEY, msg);
}

// void out(const std::string_view level, const std::string_view msg)
// {
//     console::out::file(level, msg);
//     console::out::console(level, msg);
// }

}  // namespace console
// --------------------------------------------------------------------