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

#include "entt/entt.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

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

/**
 * Transform component for storing position, rotation, and scale of entities.
 */
struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct TextureComponent {
    std::string path;
};

struct RectangleComponent {
    int width = 0;
    int height = 0;
};

struct ButtonComponent {
    std::string label;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Default color is white
    bool isPressed = false; // State of the button
    bool isHovered = false; // State of the button when hovered
    bool isPressedInside = false; // True if mouse is pressed inside the button
    // Called when mouse goes from not-hovered -> hovered
    std::function<void(entt::entity)> onHoverEnter = nullptr;
    // Called when mouse goes from hovered -> not-hovered
    std::function<void(entt::entity)> onHoverExit = nullptr;
    // Called on mouse-up while hovered (i.e., a click)
    std::function<void(entt::entity)> onClick = nullptr;
};

#endif //COMPONENTS_H
