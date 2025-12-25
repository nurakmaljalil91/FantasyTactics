/**
 * @file    Logger.h
 * @brief   Header file for the Logger class.
 * @details This file contains the definition of the Logger class which is responsible for logging messages to the console and a log file.
 *          The Logger class uses the spdlog library to log messages.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-27
 */
#ifndef FANTASYTACTICS_LOGGER_H
#define FANTASYTACTICS_LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace cbit {
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
}

#endif //FANTASYTACTICS_LOGGER_H
