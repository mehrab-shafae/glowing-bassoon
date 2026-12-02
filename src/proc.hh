#pragma once
#include "core.hpp"
// --------------------------------------------------------------------
std::string exec(std::string cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}
// --------------------------------------------------------------------

std::string it() {
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (length != -1) {
        buffer[length] = '\0';
        return std::string(buffer);
    }
    return "";
}
// --------------------------------------------------------------------

std::string getDirectory() {
    std::string executablePath = it();
    std::filesystem::path path(executablePath);
    return path.parent_path().string();
}
// --------------------------------------------------------------------
