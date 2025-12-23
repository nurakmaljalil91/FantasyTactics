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
#include "graphics/meshes/Cube.h"

#pragma region DefaultComponents
/**
 * @struct TagComponent
 * @brief  Tag component for identifying entities.
 */
struct TagComponent {
    std::string tag;
};

/**
 * @struct IdComponent
 * @brief  Identifier component for entities.
 */
struct IdComponent {
    std::string uuid;
};

/**
 * @struct TransformComponent
 * @brief  Transform component for storing position, rotation, and scale of entities.
 */
struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

#pragma endregion DefaultComponents

/**
 * @struct TextureComponent
 * @brief  Texture component for storing a texture path.
 */
struct TextureComponent {
    std::string path;
};

/**
 * @struct RectangleComponent
 * @brief  Rectangle component for storing width and height.
 */
struct RectangleComponent {
    int width = 0;
    int height = 0;
};

#pragma region UIComponent

/**
 * @enum  UIAnchor
 * @brief Enumeration for UI anchor positions.
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
 * @struct UIAnchorComponent
 * @brief UI Anchor component for positioning UI elements.
 *
 * This component defines the anchor point, offset, size, z-index, visibility,
 * and interactivity of a UI element.
 */
struct UIAnchorComponent {
    /** Anchor point for the UI element */
    UIAnchor anchor = UIAnchor::TopLeft;
    glm::vec2 offsetPixel{0.0f, 0.0f}; // Offset in pixels from the anchor point
    glm::vec2 sizePixel{0.0f, 0.0f}; // Size in pixels
    int zIndex = 0; // Layering order
    bool visible = true; // Visibility flag
    bool interactable = true; // Interactivity flag
};

/**
 * @struct ButtonComponent
 * @brief  Button component for UI buttons.
 */
struct ButtonComponent {
    std::string label;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // The default color is white
    bool isPressed = false; // State of the button
    bool isHovered = false; // State of the button when hovered
    bool isPressedInside = false; // True if the mouse is pressed inside the button
    // Called when mouse goes from not-hovered -> hovered
    std::function<void(entt::entity)> onHoverEnter = nullptr;
    // Called when mouse goes from hovered -> not-hovered
    std::function<void(entt::entity)> onHoverExit = nullptr;
    // Called on mouse-up while hovered (i.e., a click)
    std::function<void(entt::entity)> onClick = nullptr;
};

/**
 * @struct UIColorRectangleComponent
 * @brief  Color component for UI rectangles.
 */
struct UIColorRectangleComponent {
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // The default color is white
    glm::vec4 hoverColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // Color when hovered
    glm::vec4 activeColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

/**
 * @struct UIImageComponent
 * @brief  Image component for UI images.
 */
struct UIImageComponent {
    std::string path;
    glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // The default tint color is white
};

/**
 * @struct UITextComponent
 * @brief  Text component for UI text elements.
 */
struct UITextComponent {
    std::string text;
    float fontSize = 1.f; // Default font size
    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // The default color is black
};

/**
 * @struct UIPointerCallbacksComponent
 * @brief  UIPointerCallbacks component for handling pointer events on UI elements.
 */
struct UIPointerCallbacksComponent {
    // Called when the mouse goes from not-hovered -> hovered
    std::function<void(entt::entity)> onHoverEnter = nullptr;
    // Called when the mouse goes from hovered -> not-hovered
    std::function<void(entt::entity)> onHoverExit = nullptr;
    // Called on mouse-up while hovered (i.e., a click)
    std::function<void(entt::entity)> onClick = nullptr;
    // Called on mouse-up (release event)
    std::function<void(entt::entity)> onRelease = nullptr;
    // Called on mouse-down (press event)
    std::function<void(entt::entity)> onPress = nullptr;
};

/**
 * @struct UIPointerState
 * @brief  UIPointerState component for tracking pointer state on UI elements.
 */
struct UIPointerState {
    bool hovered = false;
    bool pressed = false;
    bool pressedInside = false;
};

#pragma endregion UIComponent

#pragma region MeshComponents

/**
 * @struct CubeComponent
 * @brief  Cube component for storing a Cube mesh.
 */
struct CubeComponent {
    Cube cube;
};

#pragma endregion MeshComponents
#endif //COMPONENTS_H
