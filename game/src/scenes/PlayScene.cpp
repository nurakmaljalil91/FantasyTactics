/**
 * @file    PlayScene.cpp
 * @brief   PlayScene class for managing the gameplay scene.
 * @details The PlayScene class is responsible for managing the gameplay of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-02
 */

#include "PlayScene.h"
#include "core/Input.h"
#include "ecs/GameObject.h"
#include "glm/gtc/type_ptr.inl"

void PlayScene::initialize() {
    auto mainCamera = getWorld().createGameObject("MainCamera")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::CameraComponent>()
            .addComponent<cbit::ActiveCameraComponent>();

    auto &cameraComponent = mainCamera.getComponent<cbit::CameraComponent>();
    cameraComponent.type = cbit::CameraType::Isometric;

    // Classic isometric view angles
    cameraComponent.yaw = 45.0f;
    cameraComponent.pitch = 35.264f;

    // Orthographic size
    cameraComponent.orthoLeft = -10.0f;
    cameraComponent.orthoRight = 10.0f;
    cameraComponent.orthoBottom = -10.0f;
    cameraComponent.orthoTop = 10.0f;

    auto &cameraTransformComponent = mainCamera.getComponent<cbit::TransformComponent>();

    cameraTransformComponent.position = cbit::Vector3{-10.0f, -10.0f, -10.0f};

    getWorld().createGameObject("MainLighting")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::DirectionalLightComponent>();

    getWorld().createGameObject("Cube")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::CubeComponent>();
}


void PlayScene::update(float deltaTime) {
    auto player = getWorld().getGameObject("Cube");
    auto &transformComponent = player.getComponent<cbit::TransformComponent>();

    if (cbit::Input::isKeyDown(cbit::Keyboard::A)) {
        transformComponent.position.x -= 5.0f * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::D)) {
        transformComponent.position.x += 5.0f * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::W)) {
        transformComponent.position.z -= 5.0f * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::S)) {
        transformComponent.position.z += 5.0f * deltaTime;
    }
}

void PlayScene::render() {
    Scene::render();
}
