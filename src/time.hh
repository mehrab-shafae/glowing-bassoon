#pragma once
#include "core.hpp"
// --------------------------------------------------------------------
#ifndef LOG_TIME_Z
#define LOG_TIME_Z "%H:%M-%d.%m.%y+%Z"
#endif
// --------------------------------------------------------------------
std::string get_timestamp()
{
    const auto time_point = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_point), LOG_TIME_Z);
    return ss.str();
}
// --------------------------------------------------------------------
// inline std::chrono::duration<double> time()
// {
// // std::chrono::high_resolution_clock::now(); const std::chrono::duration<double> elapsed = end_time - start_time; std::cout << std::fixed << std::setprecision(6); echo("high-clock end: " << elapsed.count() << "s")
//     return std::chrono::high_resolution_clock::now();
// }
// --------------------------------------------------------------------