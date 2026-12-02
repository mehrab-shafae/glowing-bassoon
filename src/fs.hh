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

bool touch(const std::string_view fullName)
{
        if (fullName == "") return false;

        const std::string path = std::format("{}/{}", DATA_FOLDER_PATH, fullName);
        std::ofstream dbFile{path, std::ios::app};
        if(dbFile.is_open())
        {
            dbFile << "";
            return true;
        }
        return false;
}

} // namespace touch
// --------------------------------------------------------------------
class FileManager final {
public:
    FileManager(const FileManager &) = delete;
    FileManager &operator=(const FileManager &) = delete;

    FileManager() noexcept : publicDir("./public_html") {}

    std::optional<std::filesystem::path> get_valid_path(const std::string& path) const noexcept {
        try {
            auto filePath = std::filesystem::absolute(path).lexically_normal();
            const auto baseDir = std::filesystem::absolute(publicDir).lexically_normal();

            if (!filePath.has_root_path() || !baseDir.has_root_path() ||
                filePath.string().rfind(baseDir.string(), 0) != 0 ||
                !is_valid_file(filePath)) {
                return std::nullopt;
            }

            return filePath;
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<std::string> read_file_content(const std::filesystem::path& path) const noexcept {
        try{
            std::ifstream file(path, std::ios::binary);
            if (!file) {
                return std::nullopt;
            }

            std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            return file.bad() ? std::nullopt : std::optional<std::string>(std::move(result));
        } catch (...) {
            return std::nullopt;
        }
    }

    __attribute__((hot)) std::optional<std::string> readFile(const std::string& filePath) noexcept {
        try{
            const auto filePathOpt = get_valid_path(filePath);
            if (!filePathOpt) {
                return std::nullopt;
            }

            const auto& filePathStr = filePathOpt->string();
            auto it = cache.find(filePathStr);
            if (it != cache.end()) {
                return it->second;
            }

            auto result = read_file_content(*filePathOpt);
            if (result) {
                auto [it, inserted] = cache.emplace(filePathStr, std::move(*result));
                return it->second;
            }

            return std::nullopt;
        } catch (...) {
            return std::nullopt;
        }
    }

private:
    bool is_valid_file(const std::filesystem::path& path) const noexcept {
        std::error_code ec;
        const auto status = std::filesystem::status(path, ec);
        return !ec && std::filesystem::is_regular_file(status);
    }

    std::string publicDir;
    std::unordered_map<std::string, std::string> cache;
};
static FileManager fileManager;

/*
#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <pwd.h>


namespace fs = std::filesystem;

void copyDir(const fs::path &src, const fs::path &dst)
{
    try
    {
        fs::copy(src, dst, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error &e)
    {
        throw std::runtime_error("Error copying directory: " + std::string(e.what()));
    }
}

fs::path targetDir = fs::path(homeDir) / ".marboris";

        if (fs::exists(targetDir))
        {
            fs::remove_all(targetDir);
        }

        fs::create_directory(targetDir);
*/