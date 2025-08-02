/**
 * @file    MenuScene.cpp
 * @brief   MenuScene class for managing the main menu scene.
 * @details The MenuScene class is responsible for managing the main menu of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "MenuScene.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "graphics/renderers/TextRenderer.h"
#include "graphics/ui/Quad2D.h"
#include "utilities/Logger.h"

void MenuScene::initialize() {
    _uiShader.loadShaders("resources/shaders/ui.vert", "resources/shaders/ui.frag");
    _textRenderer.loadFont("resources/fonts/Amble.ttf", 24);
    _buttonTex.loadTexture("resources/textures/crate.jpg", true);
}

void MenuScene::update(float deltaTime) {
}

void MenuScene::render() {
    const glm::mat4 orthoProjection = glm::ortho(
        0.0f, static_cast<float>(1200), // left → right
        0.0f, static_cast<float>(800), // bottom → top
        -1.0f, 1.0f
    );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _uiShader.use();
    _uiShader.setUniform("uProjection", orthoProjection);
    constexpr glm::vec2 buttonPosition(100.0f, 50.0f); // Y = height - (buttonHeight + margin)
    constexpr glm::vec2 buttonSize(200.0f, 64.0f); // Size in pixels
    _buttonTex.bind(0);
    _uiShader.setUniform("uTexture", 0);
    _uiShader.setUniform("uPosition", buttonPosition);
    _uiShader.setUniform("uSize", buttonSize);
    // Draw the quad as a button
    _buttonQuad.draw();

    constexpr float padding = 10.0f;
    _textRenderer.renderText(
        "Play",
        buttonPosition.x + padding,
        buttonPosition.y + padding,
        1.0f,
        glm::vec3(1.0f) // white text
    );

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
