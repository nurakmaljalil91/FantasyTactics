/**
 * @file    Logger.cpp
 * @brief   This file contains the implementation of the Logger class.
 * @details The Logger class is a singleton class used to log messages to the console and to a file.
 *          The logger is implemented using the spdlog library.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-27
 */

#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> cbit::Logger::mLogger;

void cbit::Logger::initialize() {
    const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/logfile.txt", true);

    // create a logger with two sinks
    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    mLogger = std::make_shared<spdlog::logger>("Cbit Engine", begin(sinks), end(sinks));

    // set pattern for logging
    spdlog::set_pattern("%^[%T] %n: %v%$");

    // set the logging level
    mLogger->set_level(spdlog::level::trace);
}
