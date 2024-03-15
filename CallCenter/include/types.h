#pragma once
#include <chrono>
#include <nlohmann/json.hpp>
#include <cstdarg>
#include <P7_Trace.h>

using ms_t = uint64_t;
using opID_t = uint16_t;
using json = nlohmann::json;
using time_point = std::chrono::time_point<std::chrono::system_clock>;
using duration_t = std::chrono::duration<double>;
