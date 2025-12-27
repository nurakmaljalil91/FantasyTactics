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

#include <string>

void PlayScene::initialize() {
    auto mainCamera = getWorld().createGameObject("MainCamera")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::CameraComponent>()
            .addComponent<cbit::ActiveCameraComponent>();

    auto &cameraComponent = mainCamera.getComponent<cbit::CameraComponent>();
    cameraComponent.type = cbit::CameraType::Isometric;

    // Classic isometric view angles
    cameraComponent.yaw = 45.0f;
    cameraComponent.pitch = -35.264f;

    // Orthographic size
    cameraComponent.orthoLeft = -10.0f;
    cameraComponent.orthoRight = 10.0f;
    cameraComponent.orthoBottom = -10.0f;
    cameraComponent.orthoTop = 10.0f;

    auto &cameraTransformComponent = mainCamera.getComponent<cbit::TransformComponent>();

    cameraTransformComponent.position = cbit::Vector3{-10.0f, 10.0f, -10.0f};

    auto mainLight = getWorld().createGameObject("MainLighting")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::DirectionalLightComponent>();

    auto &lightComponent = mainLight.getComponent<cbit::DirectionalLightComponent>();
    lightComponent.direction = cbit::Vector3{-0.6f, -1.0f, -0.5f};
    lightComponent.ambient = cbit::Vector3{0.3f, 0.3f, 0.3f};
    lightComponent.diffuse = cbit::Vector3{1.0f, 1.0f, 1.0f};
    lightComponent.intensity = 1.6f;

    constexpr int gridWidth = 8;
    constexpr int gridHeight = 8;
    constexpr float tileSize = 1.0f;

    constexpr float originX = -((gridWidth - 1) * tileSize) * 0.5f;
    constexpr float originZ = -((gridHeight - 1) * tileSize) * 0.5f;

    for (int z = 0; z < gridHeight; ++z) {
        for (int x = 0; x < gridWidth; ++x) {
            const int heightMap[gridHeight][gridWidth] = {
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0, 0, 0, 0},
                {0, 1, 1, 1, 1, 0, 0, 0},
                {0, 1, 1, 1, 1, 1, 0, 0},
                {0, 0, 1, 1, 1, 1, 0, 0},
                {0, 0, 0, 1, 1, 1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}
            };
            const int height = heightMap[z][x];
            for (int y = 0; y <= height; ++y) {
                constexpr float tileHeight = 1.0f;
                auto tile = getWorld().createGameObject(
                    "Tile_" + std::to_string(x) + "_" + std::to_string(z) + "_" + std::to_string(y))
                    .addComponent<cbit::TransformComponent>()
                    .addComponent<cbit::CubeComponent>();

                auto &tileTransform = tile.getComponent<cbit::TransformComponent>();
                tileTransform.position = cbit::Vector3{
                    originX + static_cast<float>(x) * tileSize,
                    static_cast<float>(y) * tileHeight,
                    originZ + static_cast<float>(z) * tileSize
                };
            }
        }
    }
}


void PlayScene::update(float deltaTime) {
    auto player = getWorld().getGameObject("MainLighting");
    auto &transformComponent = player.getComponent<cbit::DirectionalLightComponent>();

    if (cbit::Input::isKeyDown(cbit::Keyboard::A)) {
        transformComponent.direction.x -= 5.0f * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::D)) {
        transformComponent.direction.x += 5.0f * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::W)) {
        transformComponent.direction.z -= 5.0f * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::S)) {
        transformComponent.direction.z += 5.0f * deltaTime;
    }

    if (cbit::Input::isMouseDown(cbit::MouseButton::Left)) {
        double mouseX, mouseY;
        cbit::Input::getMousePosition(mouseX, mouseY);
        (void) mouseX;
        (void) mouseY;
        // Implement mouse drag to rotate the camera if needed
        cbit::Logger::log()->info("Mouse Position: ({}, {})", mouseX, mouseY);
    }
}

void PlayScene::render() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    Scene::render();
}
