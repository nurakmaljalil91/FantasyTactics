/**
 * @file    UISystem.h
 * @brief   UISystem class for managing user interface rendering in the entity-component system.
 * @details The UISystem class is responsible for rendering user interface elements in the entity-component system.
 *          It uses the entt library for efficient entity management and component storage.
 *          The UISystem can be extended in the future to include methods for handling user input,
 *          managing UI components, and integrating with other systems such as event handling and animation.
 *          It can be used to organize UI elements, handle user interactions, and manage rendering and updates for the user interface.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *          The UISystem can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete user interface
 *          management system. It can be used to create menus, HUDs, and other UI elements that enhance the user experience in the game or application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "Components.h"
#include "entt/entt.hpp"
#include "graphics/renderers/ShaderProgram.h"
#include "graphics/renderers/TextRenderer.h"
#include "graphics/renderers/Texture.h"
#include "graphics/ui/Quad2D.h"

/**
 * @class UISystem
 * @brief Represents the user interface system for rendering UI elements in the entity-component system.
 * @details The UISystem class is responsible for rendering user interface elements in the entity-component system.
 *          It uses the entt library for efficient entity management and component storage.
 *          The UISystem can be extended in the future to include methods for handling user input,
 *          managing UI components, and integrating with other systems such as event handling and animation.
 *          It can be used to organize UI elements, handle user interactions, and manage rendering and updates for the user interface.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *          The UISystem can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete user interface
 *          management system. It can be used to create menus, HUDs, and other UI elements that enhance the user experience in the game or application.
 * @note This class is part of the entity-component system (ECS) architecture and is designed to work with the entt library for
 *       efficient entity management and component storage.
 */
class UISystem {
public:
    /**
     * @brief Constructs a UISystem instance with the given registry.
     * @param window The GLFW window to be used for rendering the user interface.
     * @param registry The entt registry to be used for managing entities and components.
     */
    explicit UISystem(GLFWwindow *window, entt::registry &registry);

    /**
     * Set window for the UISystem.
     * @details This method sets the GLFW window that will be used by the UISystem for rendering and handling user input.
     *          It is typically called during the initialization phase of the application.
     *          The UISystem will use this window to render UI elements and handle mouse events.
     * @note This method should be called before any rendering or input handling operations are performed.
     * @param window
     */
    void setWindow(GLFWwindow *window);
    /**
     * @brief Updates the UISystem.
     * @details This method is called to update the UISystem, allowing it to process any necessary logic or state changes.
     *          Currently, it does not perform any operations but can be extended in the future.
     * @param deltaTime The time elapsed since the last update, used for time-based calculations.
     */
    void update(float deltaTime);

    /**
     * @brief Renders the user interface elements.
     * @details This method iterates through all entities with TransformComponent and TextureComponent,
     *          and draws the corresponding UI elements using the specified shaders.
     */
    void render();

private:
    GLFWwindow *_window{nullptr};
    entt::registry &_registry;
    ShaderProgram _uiShader;
    ShaderProgram _uiColorShader;
    TextRenderer _textRenderer;
    Quad2D _quad2D;
    std::unordered_map<std::string, Texture> _textures;

    // Input state
    bool _mouseDownLastFrame = false; // Track if the mouse was down in the last frame
    double _mouseX = 0.0; // Mouse X position
    double _mouseY = 0.0; // Mouse Y position

    bool _hitTest(const TransformComponent &transformComponent, const RectangleComponent &rectangleComponent,
                  double uiX, double uiY) const;


    /**
     * Draws a quad with the specified texture and transform.
     * @param texturePath The path to the texture to be used for the quad.
     * @param transform The transform component containing position, rotation, and scale.
     */
    void _drawQuad(std::string &texturePath, const TransformComponent &transform);

    /**
     * Draws a colored quad with the specified transform and color.
     * @param transform The transform component containing position, rotation, and scale.
     * @param color The color to be used for the quad.
     */
    void _drawColorQuad(const TransformComponent &transform, const glm::vec4 &color);
};


#endif //UISYSTEM_H
