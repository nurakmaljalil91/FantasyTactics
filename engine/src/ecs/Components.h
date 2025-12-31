/**
 * @file    Components.h
 * @brief   Header file for the Components struct.
 * @details This file contains the definition of the Components struct which is responsible
 *          for storing the components of an entity.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef CBIT_COMPONENTS_H
#define CBIT_COMPONENTS_H

#include <string>
#include "entt/entt.hpp"
#include "graphics/meshes/Circle.h"
#include "graphics/meshes/Cube.h"
#include "graphics/meshes/Ellipsoid.h"
#include "graphics/meshes/Mesh.h"
#include "graphics/meshes/Quad.h"
#include "graphics/meshes/Sphere.h"
#include "math/Color.h"
#include "math/Vector2.h"
#include "math/Vector3.h"

namespace cbit {
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
        Vector3 position{0.0f, 0.0f, 0.0f};
        Vector3 rotation{0.0f, 0.0f, 0.0f};
        Vector3 scale{1.0f, 1.0f, 1.0f};
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

    /**
     * @struct ShaderOverrideComponent
     * @brief  Shader override component for per-entity shader selection.
     */
    struct ShaderOverrideComponent {
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
    };

    /**
     * @struct BaseColorComponent
     * @brief  Per-entity base color used by shaders that support baseColor.
     */
    struct BaseColorComponent {
        Vector3 color{0.83f, 0.83f, 0.83f};
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
        Vector2 offsetPixel{0.0f, 0.0f}; // Offset in pixels from the anchor point
        Vector2 sizePixel{0.0f, 0.0f}; // Size in pixels
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
        Color color{1.0f, 1.0f, 1.0f, 1.0f}; // The default color is white
        bool isPressed = false; // State of the button
        bool isHovered = false; // State of the button when hovered
        bool isPressedInside = false; // True if the mouse is pressed inside the button
        // Called when the mouse goes from not-hovered -> hovered
        std::function<void(entt::entity)> onHoverEnter = nullptr;
        // Called when the mouse goes from hovered -> not-hovered
        std::function<void(entt::entity)> onHoverExit = nullptr;
        // Called on mouse-up while hovered (i.e., a click)
        std::function<void(entt::entity)> onClick = nullptr;
    };

    /**
     * @struct UIColorRectangleComponent
     * @brief  Color component for UI rectangles.
     */
    struct UIColorRectangleComponent {
        Color color{1.0f, 1.0f, 1.0f, 1.0f}; // The default color is white
        Color hoverColor{0.8f, 0.8f, 0.8f, 1.0f}; // Color when hovered
        Color activeColor{1.0f, 1.0f, 1.0f, 1.0f};
    };

    /**
     * @struct UIImageComponent
     * @brief  Image component for UI images.
     */
    struct UIImageComponent {
        std::string path;
        Color tintColor{1.0f, 1.0f, 1.0f, 1.0f}; // The default tint color is white
    };

    /**
     * @struct UITextComponent
     * @brief  Text component for UI text elements.
     */
    struct UITextComponent {
        std::string text;
        float fontSize = 1.f; // Default font size
        Color color{0.0f, 0.0f, 0.0f, 1.0f}; // The default color is black
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

    /**
     * @struct CircleComponent
     * @brief  Circle component for storing a Circle mesh.
     */
    struct CircleComponent {
        Circle circle;

        CircleComponent() = default;

        explicit CircleComponent(float radius, int segments = 32)
            : circle(radius, segments) {
        }
    };

    /**
     * @struct QuadComponent
     * @brief  Quad component for storing a Quad mesh.
     */
    struct QuadComponent {
        Quad quad;
    };

    /**
     * @struct SphereComponent
     * @brief  Sphere component for storing a Sphere mesh.
     */
    struct SphereComponent {
        Sphere sphere;

        SphereComponent() = default;

        explicit SphereComponent(float radius, int stacks = 16, int slices = 32)
            : sphere(radius, stacks, slices) {
        }
    };

    /**
     * @struct EllipsoidComponent
     * @brief  Ellipsoid component for storing an Ellipsoid mesh.
     */
    struct EllipsoidComponent {
        Ellipsoid ellipsoid;

        EllipsoidComponent(float radiusX = 1.0f, float radiusY = 1.0f, float radiusZ = 1.0f,
                           int sectorCount = 36, int stackCount = 18)
            : ellipsoid(radiusX, radiusY, radiusZ, sectorCount, stackCount) {
        }
    };

    /**
     * @struct MeshComponent
     * @brief  Mesh component for storing a Mesh loaded from file.
     */
    struct MeshComponent {
        Mesh mesh;
        std::string path;

        MeshComponent() = default;

        explicit MeshComponent(const std::string &meshPath)
            : path(meshPath) {
            mesh.loadFromFile(meshPath);
        }
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
        Vector3 direction = Vector3(-0.2f, -1.0f, -0.3f); // Direction the light is pointing

        // Light colors
        Vector3 ambient = Vector3(0.05f, 0.05f, 0.05f); // Ambient light contribution
        Vector3 diffuse = Vector3(0.4f, 0.4f, 0.4f); // Diffuse light contribution
        Vector3 specular = Vector3(0.5f, 0.5f, 0.5f); // Specular light contribution

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
        Vector3 ambient = Vector3(0.05f, 0.05f, 0.05f); // Ambient light contribution
        Vector3 diffuse = Vector3(0.8f, 0.8f, 0.8f); // Diffuse light contribution
        Vector3 specular = Vector3(1.0f, 1.0f, 1.0f); // Specular light contribution

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
        Vector3 direction = Vector3(0.0f, -1.0f, 0.0f); // Direction the spotlight is pointing

        // Light colors
        Vector3 ambient = Vector3(0.0f, 0.0f, 0.0f); // Ambient light contribution
        Vector3 diffuse = Vector3(1.0f, 1.0f, 1.0f); // Diffuse light contribution
        Vector3 specular = Vector3(1.0f, 1.0f, 1.0f); // Specular light contribution

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
}
#endif //CBIT_COMPONENTS_H
