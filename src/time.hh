#pragma once
#include "core.hpp"
// --------------------------------------------------------------------
#define LOG_TIME_Z "%H:%M-%d.%m.%y+%Z"
// --------------------------------------------------------------------
std::string get_timestamp()
{
    const auto time_point = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_point), LOG_TIME_Z);
    return ss.str();
}
// --------------------------------------------------------------------