//
// Created by User on 1/1/2024.
//

#ifndef FANTASYTACTICS_LOGGER_H
#define FANTASYTACTICS_LOGGER_H


#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger {
public:
    static Logger &get() {
        static Logger instance;
        return instance;
    }

    static void initialize();

    static std::shared_ptr<spdlog::logger> log() { return mLogger; }

private:
    Logger() = default;

    static std::shared_ptr<spdlog::logger> mLogger;
};


#endif //FANTASYTACTICS_LOGGER_H
