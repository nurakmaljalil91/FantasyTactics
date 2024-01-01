//
// Created by User on 1/1/2024.
//

#include "logger.h"

std::shared_ptr<spdlog::logger> logger::coreLogger;

void logger::init() {

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/logfile.txt", true);

    // create logger with two sinks
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    coreLogger = std::make_shared<spdlog::logger>("Fantasy Tactics", begin(sinks), end(sinks));

    // set pattern for logging
    spdlog::set_pattern("%^[%T] %n: %v%$");

    // set logging level
    coreLogger->set_level(spdlog::level::trace);
}
