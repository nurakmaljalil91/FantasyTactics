/**
 * @file    PlayScene.cpp
 * @brief   PlayScene class for managing the gameplay scene.
 * @details The PlayScene class is responsible for managing the gameplay of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-02
 */
#include "PlayScene.h"

#include "ecs/GameObject.h"
#include "glm/gtc/type_ptr.inl"

void PlayScene::initialize() {
    auto mainCamera = getWorld().createGameObject("MainCamera")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::CameraComponent>()
            .addComponent<cbit::ActiveCameraComponent>();

    auto &cameraTransformComponent = mainCamera.getComponent<cbit::TransformComponent>();

    cameraTransformComponent.position = cbit::Vector3{0.0f, 0.0f, 5.0f};

    getWorld().createGameObject("MainLighting")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::DirectionalLightComponent>();

    getWorld().createGameObject("Cube")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::CubeComponent>();


}


void PlayScene::update(float deltaTime) {
    auto player = getWorld().getGameObject("MainCamera");
    auto &transformComponent = player.getComponent<cbit::TransformComponent>();
    transformComponent.position.x += 0.05f * deltaTime;
}

void PlayScene::render() {

    Scene::render();
}
