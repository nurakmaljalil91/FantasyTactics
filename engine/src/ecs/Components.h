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

#pragma region GraphicsComponents
/**
 * @struct TextureComponent
 * @brief  Texture component for storing a texture path.
 */
struct TextureComponent {
    std::string path;
};

#pragma endregion GraphicsComponents

#pragma region ShapeComponents

/**
 * @struct RectangleComponent
 * @brief  Rectangle component for storing width and height.
 */
struct RectangleComponent {
    int width = 0;
    int height = 0;
};

#pragma endregion ShapeComponents

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

#pragma region CameraComponents

/**
 * @enum CameraType
 * @brief Enumeration for camera projection types.
 */
enum class CameraType {
    Perspective,
    Orthographic,
    Isometric
};

/**
 * @struct  CameraComponent
 * @brief   Camera component for storing camera properties.
 * @details This component stores camera position, orientation, and projection settings.
 *          Attach this to an entity to make it a camera entity.
 */
struct CameraComponent {
    CameraType type = CameraType::Perspective;

    // Camera transform
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Projection settings
    float fov = 45.0f; // Field of view (for perspective)
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float aspectRatio = 16.0f / 9.0f;

    // Orthographic/Isometric settings
    float orthoLeft = -10.0f;
    float orthoRight = 10.0f;
    float orthoBottom = -10.0f;
    float orthoTop = 10.0f;

    // Camera movement settings
    float movementSpeed = 2.5f;
    float mouseSensitivity = 0.1f;

    // Euler angles for rotation (in degrees)
    float yaw = -90.0f;
    float pitch = 0.0f;
};

/**
 * @struct  ActiveCameraComponent
 * @brief   Tag the component to mark the active camera.
 * @details Add this component to the camera entity that should be used for rendering.
 *          Only one camera should have this component at a time.
 */
struct ActiveCameraComponent {
    // Empty tag component
};

#pragma endregion CameraComponents

#pragma region LightingComponents

/**
 * @struct  DirectionalLightComponent
 * @brief   Directional light component for simulating distant light sources (e.g., sun, moon).
 * @details Directional lights have no position, only direction. They affect all objects equally
 *          regardless of distance, simulating infinitely distant light sources.
 */
struct DirectionalLightComponent {
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f); // Direction the light is pointing

    // Light colors
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f); // Ambient light contribution
    glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f); // Diffuse light contribution
    glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f); // Specular light contribution

    float intensity = 1.0f; // Light intensity multiplier
    bool enabled = true; // Whether the light is active
};

/**
 * @struct  PointLightComponent
 * @brief   Point light component for omnidirectional light sources (e.g., light bulbs, torches).
 * @details Point lights emit light in all directions from a specific position.
 *          Their intensity diminishes with distance based on attenuation factors.
 */
struct PointLightComponent {
    // Light colors
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f); // Ambient light contribution
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f); // Diffuse light contribution
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f); // Specular light contribution

    // Attenuation factors (for distance falloff)
    float constant = 1.0f; // Constant attenuation
    float linear = 0.09f; // Linear attenuation
    float quadratic = 0.032f; // Quadratic attenuation

    float intensity = 1.0f; // Light intensity multiplier
    bool enabled = true; // Whether the light is active

    // Note: Position is taken from the entity's TransformComponent
};

/**
 * @struct  SpotLightComponent
 * @brief   Spotlight component for cone-shaped light sources (e.g., flashlights, spotlights).
 * @details Spotlights emit light in a specific direction within a cone shape.
 *          They have both position and direction, with adjustable cone angles.
 */
struct SpotLightComponent {
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); // Direction the spotlight is pointing

    // Light colors
    glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f); // Ambient light contribution
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Diffuse light contribution
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f); // Specular light contribution

    // Attenuation factors (for distance falloff)
    float constant = 1.0f; // Constant attenuation
    float linear = 0.09f; // Linear attenuation
    float quadratic = 0.032f; // Quadratic attenuation

    // Spotlight cone angles (in degrees)
    float cutOff = 12.5f; // Inner cone angle (full brightness)
    float outerCutOff = 15.0f; // Outer cone angle (fade to zero)

    float intensity = 1.0f; // Light intensity multiplier
    bool enabled = true; // Whether the light is active

    // Note: Position is taken from the entity's TransformComponent
};

#pragma endregion LightingComponents

#endif //COMPONENTS_H
