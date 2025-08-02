/**
 * @file    Components.h
 * @brief   Header file for the Components struct.
 * @details This file contains the definition of the Components struct which is responsible
 *          for storing the components of an entity.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

/**
 * Tag component for identifying entities.
 */
struct TagComponent {
    std::string tag;
};

/**
 * Identifier component for entities.
 */
struct IdComponent {
    std::string uuid;
};

#endif //COMPONENTS_H
