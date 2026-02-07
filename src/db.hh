#pragma once
#include "core.hpp"
#include "fs.hh"
// --------------------------------------------------------------------
#ifndef FORMAT_DB_FILE
#define FORMAT_DB_FILE "db"
#endif
// --------------------------------------------------------------------
namespace db::unit0 {

bool join(const std::string_view dbName)
{
        const std::string_view path = std::format("{}.{}", dbName, FORMAT_DB_FILE);
        return fs::out::touch(path);
}

} // namespace unit0
// --------------------------------------------------------------------