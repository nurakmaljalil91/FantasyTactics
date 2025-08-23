/**
 * @file    MenuScene.cpp
 * @brief   MenuScene class for managing the main menu scene.
 * @details The MenuScene class is responsible for managing the main menu of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "MenuScene.h"

#include "ecs/Components.h"
#include "ecs/GameObject.h"

void MenuScene::initialize() {
    constexpr float titleWidth = 512.0f;
    // assuming your logo image is 512×128px
    constexpr float titleHeight = 128.0f;

    // compute the bottom-left so that the quad is centered
    constexpr float screenW = 1200.0f;
    constexpr float screenH = 800.0f;

    constexpr float px = (screenW - titleWidth) * 0.5f;
    constexpr float py = (screenH - titleHeight) * 0.5f;

    getWorld().createGameObject("FantasyTacticTitle")
            .addComponent<TransformComponent>(
                glm::vec3(px, py, 0.0f), // Position
                glm::vec3(0.0f, 0.0f, 0.0f), // Rotation
                glm::vec3(titleWidth, titleHeight, 1.0f) // Scale
            ).addComponent<TextureComponent>("assets/branding/fantasy_tactic_title.png");


    _button = getWorld().createGameObject("StartButton")
            .addComponent<TransformComponent>(
                glm::vec3(px - 200, py - 200, 0.0f),
                glm::vec3(0.f, 0.f, 0.0f),
                glm::vec3(titleWidth, titleHeight, 1.0f)
            ).addComponent<RectangleComponent>(240, 50).addComponent<ButtonComponent>();

    auto &button = _button.getComponent<ButtonComponent>();

    button.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    button.label = "START";
    // Handlers
    button.onHoverEnter = [&](entt::entity) { Logger::log()->info("Hover enter: Start"); };
    button.onHoverExit  = [&](entt::entity) { Logger::log()->info("Hover exit: Start"); };
    button.onClick      = [&](entt::entity) { Logger::log()->info("CLICK: Start button"); };
}

void MenuScene::update(float deltaTime) {
    Scene::update(deltaTime);
}

void MenuScene::render() {
    // set background white
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene::render();
}
