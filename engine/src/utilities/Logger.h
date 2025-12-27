/**
 * @file    Logger.h
 * @brief   Header file for the Logger class.
 * @details This file contains the definition of the Logger class which is responsible for logging messages to the console and a log file.
 *          The Logger class uses the spdlog library to log messages.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-27
 */
#ifndef CBIT_LOGGER_H
#define CBIT_LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace cbit {
    /**
     * @class Logger
     * @brief Singleton class for logging messages using spdlog.
     * @details The Logger class provides a global logging instance that can be used throughout the application.
     *          It initializes a console logger and a file logger to log messages to both the console and a log file.
     *          The log file is named "cbit_log.txt" and is created in the current working directory.
     */
    class Logger {
    public:
        /**
         * @brief Get the singleton instance of the Logger.
         * @return Reference to the Logger instance.
         */
        static Logger &get() {
            static Logger instance;
            return instance;
        }

        /**
         * @brief Initialize the logger.
         * @details This method sets up the console and file loggers using spdlog.
         *          It should be called once at the start of the application.
         */
        static void initialize();

        /**
         * @brief Get the spdlog logger instance.
         * @return Shared pointer to the spdlog logger.
         */
        static std::shared_ptr<spdlog::logger> log() { return mLogger; }

    private:
        Logger() = default;

        static std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //CBIT_LOGGER_H
