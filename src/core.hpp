#pragma once
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
#include <format>
#include <vector>
#include <optional>
#include <any>
#include <functional>
#include <exception>
#include <thread>
#include <cstdlib>
#include <sys/resource.h>
#include <filesystem>
#include <unordered_map>
#include <cstdarg>
#include <cstdio>
#include <unistd.h>
#include <limits.h>
#include <coroutine>

/*
#include <iostream>
#include <string>
#include <math.h>
#include <memory>
#include <any>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <type_traits>
#include <functional>
#include <chrono>
#include <exception>

// async
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
*/
#define CORE_DIE_0() std::exit(EXIT_SUCCESS)
#define CORE_DIE_1(x) std::exit(x)
#define CORE_DIE_X(x, A, B, FUNC, ...) FUNC
#define die(...) CORE_DIE_X(, ##__VA_ARGS__, \
                          CORE_DIE_1(__VA_ARGS__), \
                          CORE_DIE_0(__VA_ARGS__))
//--------------------------------------------
#define panic(msg) throw std::runtime_error(msg)
//--------------------------------------------
#define inline __attribute__((always_inline)) inline
//--------------------------------------------
#define sleep(x) std::this_thread::sleep_for(std::chrono::seconds(x))
//--------------------------------------------
#define echo(x) std::cout << x
//--------------------------------------------


using namespace std::string_literals;

