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
#include "glm/vec2.hpp"
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

/**
 * Enumeration for UI anchor positions.
 */
enum class UIAnchor {
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

/**
 * UI Anchor component for positioning UI elements.
 */
struct UIAnchorComponent {
    UIAnchor anchor = UIAnchor::TopLeft;
    glm::vec2 offsetPixel{0.0f, 0.0f}; // Offset in pixels from the anchor point
    glm::vec2 sizePixel{0.0f, 0.0f}; // Size in pixels
    int zIndex = 0; // Layering order
    bool visible = true; // Visibility flag
    bool interactable = true; // Interactivity flag
};

/**
 * Color component for UI rectangles.
 */
struct UIColorRectangleComponent {
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Default color is white
    glm::vec4 hoverColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // Color when hovered
    glm::vec4 activeColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

/**
 * Image component for UI images.
 */
struct UIImageComponent {
    std::string path;
    glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Default tint color is white
};

/**
 * Text component for UI text elements.
 */
struct UITextComponent {
    std::string text;
    float fontSize = 1.f; // Default font size
    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Default color is black
};

/**
 * UIPointerCallbacks component for handling pointer events on UI elements.
 */
struct UIPointerCallbacksComponent {
    // Called when mouse goes from not-hovered -> hovered
    std::function<void(entt::entity)> onHoverEnter = nullptr;
    // Called when mouse goes from hovered -> not-hovered
    std::function<void(entt::entity)> onHoverExit = nullptr;
    // Called on mouse-up while hovered (i.e., a click)
    std::function<void(entt::entity)> onClick = nullptr;
    // Called on mouse-up (release event)
    std::function<void(entt::entity)> onRelease = nullptr;
    // Called on mouse-down (press event)
    std::function<void(entt::entity)> onPress = nullptr;
};

/**
 * UIPointerState component for tracking pointer state on UI elements.
 */
struct UIPointerState {
    bool hovered = false;
    bool pressed = false;
    bool pressedInside = false;
};

#endif //COMPONENTS_H
