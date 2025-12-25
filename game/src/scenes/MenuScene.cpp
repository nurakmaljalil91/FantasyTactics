/**
 * @file    MenuScene.cpp
 * @brief   MenuScene class for managing the main menu scene.
 * @details The MenuScene class is responsible for managing the main menu of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "MenuScene.h"

#include "core/SceneManager.h"
#include "ecs/Components.h"
#include "ecs/GameObject.h"
#include "math/Color.h"

void MenuScene::initialize() {
    getWorld().createGameObject("title")
            .addComponent<cbit::UIAnchorComponent>(
                cbit::UIAnchor::TopCenter,
                cbit::Vector2(0.0f, -120.0f),
                cbit::Vector2(512.0f, 128.0f),
                0, true, false)
            .addComponent<cbit::UIImageComponent>("assets/branding/fantasy_tactic_title.png");

    auto startButton = getWorld().createGameObject("startButton")
            .addComponent<cbit::UIAnchorComponent>(
                cbit::UIAnchor::Center,
                cbit::Vector2(0.0f, -80.0f),
                cbit::Vector2(240.0f, 50.0f),
                10, true, true)
            .addComponent<cbit::UIColorRectangleComponent>()
            .addComponent<cbit::ButtonComponent>()
            .addComponent<cbit::UITextComponent>("START", 1.0f, cbit::Color::Black);

    auto &button = startButton.getComponent<cbit::ButtonComponent>();
    button.onClick = [&](entt::entity) {
        cbit::Logger::log()->info("Start button clicked! Transitioning to GameScene...");
        _sceneManager->setActiveScene("PlayScene");
    };
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
