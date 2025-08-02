/**
 * @file    UUIDGenerator.h
 * @brief   UUIDGenerator class header file
 * @details generates UUIDs for game objects
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef UUIDGENERATOR_H
#define UUIDGENERATOR_H

#include <string>

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


#endif //UUIDGENERATOR_H
