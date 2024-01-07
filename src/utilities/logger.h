//
// Created by User on 1/1/2024.
//

#ifndef FANTASYTACTICS_LOGGER_H
#define FANTASYTACTICS_LOGGER_H


#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class logger {
public:
    static logger &get() {
        static logger instance;
        return instance;
    }

    static void initialize();

    static std::shared_ptr<spdlog::logger> log() { return coreLogger; }

private:
    logger() = default;

    static std::shared_ptr<spdlog::logger> coreLogger;
};


#endif //FANTASYTACTICS_LOGGER_H
