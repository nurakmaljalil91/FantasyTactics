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
     * @param registry The entt registry to be used for managing entities and components.
     */
    explicit UISystem(entt::registry &registry);

    /**
     * @brief Renders the user interface elements.
     * @details This method iterates through all entities with TransformComponent and TextureComponent,
     *          and draws the corresponding UI elements using the specified shaders.
     */
    void render();

private:
    entt::registry &_registry;
    ShaderProgram _uiShader;
    ShaderProgram _uiColorShader;
    TextRenderer _textRenderer;
    Quad2D _quad2D;
    std::unordered_map<std::string, Texture> _textures;

    /**
     * Draws a quad with the specified texture and transform.
     * @param texturePath The path to the texture to be used for the quad.
     * @param transform The transform component containing position, rotation, and scale.
     */
    void _drawQuad(std::string& texturePath, const TransformComponent& transform);

    /**
     * Draws a colored quad with the specified transform and color.
     * @param transform The transform component containing position, rotation, and scale.
     * @param color The color to be used for the quad.
     */
    void _drawColorQuad(const TransformComponent &transform, const glm::vec4 &color);
};


#endif //UISYSTEM_H
