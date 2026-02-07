#pragma once
#include "core.hpp"
#include "time.hh"
// --------------------------------------------------------------------
#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME "logs/core.log"
#endif

#ifndef DATA_FOLDER_PATH
#define DATA_FOLDER_PATH "data"
#endif
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
namespace fs::unit0 {

bool dbJoin(const std::string_view dbName)
{
        if (dbName == "") return false;

        const std::string path = std::format("{}/{}.data", DATA_FOLDER_PATH, dbName);
        std::ofstream dbFile{path, std::ios::app};
        if(dbFile.is_open())
        {
            dbFile << "";
            return true;
        }
        return false;
}

} // namespace unit0
// --------------------------------------------------------------------