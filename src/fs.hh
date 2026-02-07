#pragma once
#include "core.hpp"
#include "time.hh"
// --------------------------------------------------------------------
#define LOG_FILE_NAME "logs/core.log"
// --------------------------------------------------------------------
namespace fs::out {

bool console(const std::string_view level, const std::string_view msg)
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