/**
 * @file    PlayScene.cpp
 * @brief   PlayScene class for managing the gameplay scene.
 * @details The PlayScene class is responsible for managing the gameplay of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-02
 */
#include "PlayScene.h"

#include "ecs/GameObject.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"

void PlayScene::initialize() {
    _shader.loadShaders("resources/shaders/default.vert", "resources/shaders/default.frag");
    getWorld().createGameObject("Cube")
            .addComponent<TransformComponent>(
                Vector(0.0f, 0.0f, 0.0f),
                Vector(0.0f, 0.0f, 0.0f),
                Vector(1.0f, 1.0f, 1.0f)
            )
            .addComponent<CubeComponent>();
}


void PlayScene::update(float deltaTime) {
}

void PlayScene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getWorld().render(_isometricCamera, getWindowWidth(), getWindowHeight());
}
