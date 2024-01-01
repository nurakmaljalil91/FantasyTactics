//
// Created by User on 1/1/2024.
//

#ifndef FANTASYTACTICS_LOGGER_H
#define FANTASYTACTICS_LOGGER_H


#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class logger {
public:
    static void init();

    static std::shared_ptr<spdlog::logger> getCoreLogger() { return coreLogger; }
private:
    static std::shared_ptr<spdlog::logger> coreLogger;
};


#endif //FANTASYTACTICS_LOGGER_H
