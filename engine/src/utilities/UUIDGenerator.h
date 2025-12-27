/**
 * @file    UUIDGenerator.h
 * @brief   UUIDGenerator class header file
 * @details generates UUIDs for game objects
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef CBIT_UUIDGENERATOR_H
#define CBIT_UUIDGENERATOR_H

#include <string>

namespace cbit {
    /**
     * @class UUIDGenerator
     * @brief Generates unique identifiers (UUIDs) for game objects.
     */
    class UUIDGenerator {
    public:
        /**
         * Generates a new UUID.
         * @return newly generated UUID as a string.
         */
        static std::string generate();
    };
}

#endif //CBIT_UUIDGENERATOR_H
