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
#include "entt/entt.hpp"
#include <string>

namespace {
    constexpr int kGridWidth = 8;
    constexpr int kGridHeight = 8;
    constexpr float kTileSize = 1.0f;
    constexpr float kTileHeight = 1.0f;

    const int kHeightMap[kGridHeight][kGridWidth] = {
        {0, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 0, 2, 2, 2, 2},
        {0, 0, 0, 0, 2, 2, 2, 3}
    };

    void applyShaderOverrideToTiles(cbit::EntityComponentSystem &world, const bool useCelShader) {
        for (int z = 0; z < kGridHeight; ++z) {
            for (int x = 0; x < kGridWidth; ++x) {
                const int height = kHeightMap[z][x];
                for (int y = 0; y <= height; ++y) {
                    auto tile = world.getGameObject(
                        "Tile_" + std::to_string(x) + "_" + std::to_string(z) + "_" + std::to_string(y));
                    if (tile.getEntity() == entt::null) {
                        continue;
                    }
                    if (useCelShader) {
                        tile.setShaderOverride(
                            "resources/shaders/cel_shading.vert",
                            "resources/shaders/cel_shading.frag");
                    } else {
                        tile.clearShaderOverride();
                    }
                }
            }
        }
    }
}

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
    lightComponent.direction = cbit::Vector3{-0.4f, -1.0f, -0.4f};
    lightComponent.ambient = cbit::Vector3{0.55f, 0.55f, 0.55f};
    lightComponent.diffuse = cbit::Vector3{1.0f, 1.0f, 1.0f};
    lightComponent.intensity = 1.2f;

    constexpr float originX = -((kGridWidth - 1) * kTileSize) * 0.5f;
    constexpr float originZ = -((kGridHeight - 1) * kTileSize) * 0.5f;

    for (int z = 0; z < kGridHeight; ++z) {
        for (int x = 0; x < kGridWidth; ++x) {
            const int height = kHeightMap[z][x];
            for (int y = 0; y <= height; ++y) {
                auto tile = getWorld().createGameObject(
                            "Tile_" + std::to_string(x) + "_" + std::to_string(z) + "_" + std::to_string(y))
                        .addComponent<cbit::TransformComponent>()
                        .addComponent<cbit::CubeComponent>();

                auto &tileTransform = tile.getComponent<cbit::TransformComponent>();
                tileTransform.position = cbit::Vector3{
                    originX + static_cast<float>(x) * kTileSize,
                    static_cast<float>(y) * kTileHeight,
                    originZ + static_cast<float>(z) * kTileSize
                };
            }
        }
    }
}


void PlayScene::update(float deltaTime) {
    static bool useCelShader = false;
    if (cbit::Input::isKeyPressed(cbit::Keyboard::C)) {
        useCelShader = !useCelShader;
        applyShaderOverrideToTiles(getWorld(), useCelShader);
    }

    auto player = getWorld().getGameObject("MainLighting");
    auto &lightComponent = player.getComponent<cbit::DirectionalLightComponent>();

    constexpr float directionSpeed = 2.0f;
    if (cbit::Input::isKeyDown(cbit::Keyboard::A)) {
        lightComponent.direction.x -= directionSpeed * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::D)) {
        lightComponent.direction.x += directionSpeed * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::W)) {
        lightComponent.direction.z -= directionSpeed * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::S)) {
        lightComponent.direction.z += directionSpeed * deltaTime;
    }

    if (cbit::Input::isKeyDown(cbit::Keyboard::Q)) {
        lightComponent.direction.y -= directionSpeed * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::E)) {
        lightComponent.direction.y += directionSpeed * deltaTime;
    }

    constexpr float colorSpeed = 0.6f;
    const bool decrease = cbit::Input::isKeyDown(cbit::Keyboard::LeftShift)
                          || cbit::Input::isKeyDown(cbit::Keyboard::RightShift);
    const float colorDelta = (decrease ? -1.0f : 1.0f) * colorSpeed * deltaTime;

    auto clamp01 = [](float value) {
        if (value < 0.0f) {
            return 0.0f;
        }
        if (value > 1.0f) {
            return 1.0f;
        }
        return value;
    };

    if (cbit::Input::isKeyDown(cbit::Keyboard::R)) {
        lightComponent.diffuse.x = clamp01(lightComponent.diffuse.x + colorDelta);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::G)) {
        lightComponent.diffuse.y = clamp01(lightComponent.diffuse.y + colorDelta);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::B)) {
        lightComponent.diffuse.z = clamp01(lightComponent.diffuse.z + colorDelta);
    }

    if (cbit::Input::isKeyDown(cbit::Keyboard::I)) {
        lightComponent.intensity += 0.5f * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::K)) {
        lightComponent.intensity -= 0.5f * deltaTime;
        if (lightComponent.intensity < 0.0f) {
            lightComponent.intensity = 0.0f;
        }
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
