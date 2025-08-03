/**
 * @file     UISystem.cpp
 * @brief    UISystem class implementation file
 * @details  This file contains the implementation of the UISystem class, which is responsible for rendering user interface elements in the entity-component system.
 *           The UISystem uses the entt library for efficient entity management and component storage.
 *           It can be extended in the future to include methods for handling user input, managing UI components,
 *           and integrating with other systems such as event handling and animation.
 *           The UISystem can be used to organize UI elements, handle user interactions, and manage rendering and updates for the user interface.
 *           The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *           It can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete user interface management system.
 *           The UISystem can be used to create menus, HUDs, and other UI elements that enhance the user experience in the game or application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#include "UISystem.h"
#include "Components.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "utilities/Logger.h"

UISystem::UISystem(entt::registry &registry): _registry(registry), _textRenderer(1200, 800) {
    // Constructor implementation can be added here if needed
    _uiShader.loadShaders("resources/shaders/ui.vert", "resources/shaders/ui.frag");
    _uiColorShader.loadShaders("resources/shaders/color_ui.vert", "resources/shaders/color_ui.frag");
    _textRenderer.loadFont("resources/fonts/Amble.ttf", 50);
}

void UISystem::render() {
    const glm::mat4 orthoProjection = glm::ortho(
        0.0f, static_cast<float>(1200), // left → right
        0.0f, static_cast<float>(800), // bottom → top
        -1.0f, 1.0f
    );

    _uiShader.use();
    _uiShader.setUniform("uProjection", orthoProjection);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto view = _registry.view<TransformComponent, TextureComponent>();
    Logger::log()->info("Found {} UI quads", static_cast<int>(view.size_hint()));

    for (const auto entities: _registry.view<TransformComponent, TextureComponent>()) {
        auto &transform = _registry.get<TransformComponent>(entities);
        auto &texture = _registry.get<TextureComponent>(entities);

        _drawQuad(texture.path, transform);
        // _drawColorQuad(transform, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Draw a white quad for now
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void UISystem::_drawQuad(std::string &texturePath, const TransformComponent &transform) {
    auto [it, inserted] = _textures.try_emplace(texturePath);  // default‐constructs Texture
    if (inserted) {
        if (!it->second.loadTexture(texturePath)) {
            Logger::log()->error("Failed to load texture: {}", texturePath);
            _textures.erase(it);
            return;
        }
    }
    // now it->second is valid
    it->second.bind();

    _uiShader.use();

    glActiveTexture(GL_TEXTURE0);
    _uiShader.setUniform("uTexture", 0);
    glm::vec2 position{
        transform.position.x,
        transform.position.y
    };
    glm::vec2 size{
        transform.scale.x,
        transform.scale.y
    };
    _uiShader.setUniform("uPosition", position);
    _uiShader.setUniform("uSize", size);
    Logger::log()->debug("The model matrix for {} is: {}", texturePath, transform.scale.x);

    _quad2D.draw();
}

void UISystem::_drawColorQuad(const TransformComponent &transform, const glm::vec4 &color) {
    _uiColorShader.use();
    _uiColorShader.setUniform("uProjection", glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f));

    const glm::vec2 position(transform.position.x, transform.position.y);
    const glm::vec2 size(transform.scale.x, transform.scale.y);
    _uiColorShader.setUniform("uPosition", position);
    _uiColorShader.setUniform("uSize", size);
    _uiColorShader.setUniform("uColor", color);

    // draw the unit quad scaled/translated
    _quad2D.draw();
}
