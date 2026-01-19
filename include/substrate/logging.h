#pragma once

#define TRACE SPDLOG_TRACE
#define DEBUG SPDLOG_DEBUG
#define INFO SPDLOG_INFO
#define WARN SPDLOG_WARN
#define ERROR SPDLOG_ERROR
#define CRITICAL SPDLOG_CRITICAL

#include <spdlog/sinks/basic_file_sink.h>    // IWYU pragma: export
#include <spdlog/sinks/stdout_color_sinks.h> // IWYU pragma: export
#include <spdlog/spdlog.h>                   // IWYU pragma: export
