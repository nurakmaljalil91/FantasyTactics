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
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "entt/entt.hpp"
#include <algorithm>
#include <cmath>
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

    glm::mat4 buildCameraProjection(const cbit::CameraComponent &camera, const float aspectRatio) {
        switch (camera.type) {
            case cbit::CameraType::Perspective:
                return glm::perspective(
                    glm::radians(camera.fov),
                    aspectRatio,
                    camera.nearPlane,
                    camera.farPlane);
            case cbit::CameraType::Orthographic:
            case cbit::CameraType::Isometric:
                return glm::ortho(
                    camera.orthoLeft,
                    camera.orthoRight,
                    camera.orthoBottom,
                    camera.orthoTop,
                    camera.nearPlane,
                    camera.farPlane);
            default:
                return {1.0f};
        }
    }

    glm::mat4 buildCameraView(const cbit::CameraComponent &camera, const cbit::TransformComponent &transform) {
        const float yaw = glm::radians(camera.yaw);
        const float pitch = glm::radians(camera.pitch);

        glm::vec3 forward;
        forward.x = cosf(pitch) * cosf(yaw);
        forward.y = sinf(pitch);
        forward.z = cosf(pitch) * sinf(yaw);

        const glm::vec3 cameraPosition = transform.position.toGLM();
        const glm::vec3 cameraTarget = cameraPosition + glm::normalize(forward);
        constexpr auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        return glm::lookAt(cameraPosition, cameraTarget, cameraUp);
    }

    bool screenToWorldOnPlane(
        const double screenX,
        const double screenY,
        const int screenWidth,
        const int screenHeight,
        const glm::mat4 &projection,
        const glm::mat4 &view,
        const float planeY,
        glm::vec3 &outWorld) {
        if (screenWidth <= 0 || screenHeight <= 0) {
            return false;
        }

        const auto ndcX = static_cast<float>(2.0 * screenX / screenWidth - 1.0);
        const auto ndcY = static_cast<float>(1.0 - 2.0 * screenY / screenHeight);

        const glm::mat4 invViewProjection = glm::inverse(projection * view);
        glm::vec4 nearPoint = invViewProjection * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
        glm::vec4 farPoint = invViewProjection * glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

        if (nearPoint.w == 0.0f || farPoint.w == 0.0f) {
            return false;
        }

        nearPoint /= nearPoint.w;
        farPoint /= farPoint.w;

        const auto rayOrigin = glm::vec3(nearPoint);
        const glm::vec3 rayDirection = glm::normalize(glm::vec3(farPoint - nearPoint));

        if (std::abs(rayDirection.y) < 0.0001f) {
            return false;
        }

        const float t = (planeY - rayOrigin.y) / rayDirection.y;
        outWorld = rayOrigin + rayDirection * t;
        return true;
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
    lightComponent.direction = cbit::Vector3{-6.0f, -1.0f, -1.0f};
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
                        .addComponent<cbit::CubeComponent>()
                        .addComponent<cbit::TextureComponent>("assets/textures/default_cube_texture.png");

                auto &tileTransform = tile.getComponent<cbit::TransformComponent>();
                tileTransform.position = cbit::Vector3{
                    originX + static_cast<float>(x) * kTileSize,
                    static_cast<float>(y) * kTileHeight,
                    originZ + static_cast<float>(z) * kTileSize
                };
            }
        }
    }

    auto player = getWorld().createGameObject("player")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::MeshComponent>("assets/models/characterMedium.fbx");

    auto &[playerPosition, playerRotation, playerScale] = player.getComponent<cbit::TransformComponent>();
    playerPosition = cbit::Vector3{2.0f, 5.0f, 2.0f};
    playerRotation = cbit::Vector3{-250.0f, -190.0f, 0.0f};
    playerScale = cbit::Vector3{0.6f, 0.6f, 0.6f};
}


void PlayScene::update(const float deltaTime) {
    static bool useCelShader = false;
    if (cbit::Input::isKeyPressed(cbit::Keyboard::C)) {
        useCelShader = !useCelShader;
        applyShaderOverrideToTiles(getWorld(), useCelShader);
    }


    auto player = getWorld().getGameObject("player");

    auto &[position, rotation, scale] = player.getComponent<cbit::TransformComponent>();

    constexpr float rotateSpeed = 90.0f;
    if (cbit::Input::isKeyDown(cbit::Keyboard::W)) {
        rotation.y -= rotateSpeed * deltaTime;
        cbit::Logger::log()->info("Player rotation: ({}, {}, {})", rotation.x, rotation.y, rotation.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::S)) {
        rotation.y += rotateSpeed * deltaTime;
        cbit::Logger::log()->info("Player rotation: ({}, {}, {})", rotation.x, rotation.y, rotation.z);
    }

    if (cbit::Input::isKeyDown(cbit::Keyboard::A)) {
        rotation.x -= rotateSpeed * deltaTime;
        cbit::Logger::log()->info("Player rotation: ({}, {}, {})", rotation.x, rotation.y, rotation.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::D)) {
        rotation.x += rotateSpeed * deltaTime;
        cbit::Logger::log()->info("Player rotation: ({}, {}, {})", rotation.x, rotation.y, rotation.z);
    }

    if (cbit::Input::isKeyDown(cbit::Keyboard::Q)) {
        rotation.z -= rotateSpeed * deltaTime;
        cbit::Logger::log()->info("Player rotation: ({}, {}, {})", rotation.x, rotation.y, rotation.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::E)) {
        rotation.z += rotateSpeed * deltaTime;
        cbit::Logger::log()->info("Player rotation: ({}, {}, {})", rotation.x, rotation.y, rotation.z);
    }

    constexpr float scaleStep = 0.05f;
    if (cbit::Input::isKeyDown(cbit::Keyboard::P)) {
        scale.x += scaleStep;
        scale.y += scaleStep;
        scale.z += scaleStep;
        cbit::Logger::log()->info("Player scale: ({}, {}, {})", scale.x, scale.y, scale.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::L)) {
        scale.x = std::max(0.01f, scale.x - scaleStep);
        scale.y = std::max(0.01f, scale.y - scaleStep);
        scale.z = std::max(0.01f, scale.z - scaleStep);
        cbit::Logger::log()->info("Player scale: ({}, {}, {})", scale.x, scale.y, scale.z);
    }

    constexpr float moveSpeed = 5.0f;
    if (cbit::Input::isKeyDown(cbit::Keyboard::Up)) {
        position.z -= moveSpeed * deltaTime;
        cbit::Logger::log()->info("Player position: ({}, {}, {})", position.x, position.y, position.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::Down)) {
        position.z += moveSpeed * deltaTime;
        cbit::Logger::log()->info("Player position: ({}, {}, {})", position.x, position.y, position.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::Left)) {
        position.x -= moveSpeed * deltaTime;
        cbit::Logger::log()->info("Player position: ({}, {}, {})", position.x, position.y, position.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::Right)) {
        position.x += moveSpeed * deltaTime;
        cbit::Logger::log()->info("Player position: ({}, {}, {})", position.x, position.y, position.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::Y)) {
        position.y += moveSpeed * deltaTime;
        cbit::Logger::log()->info("Player position: ({}, {}, {})", position.x, position.y, position.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::H)) {
        position.y -= moveSpeed * deltaTime;
        cbit::Logger::log()->info("Player position: ({}, {}, {})", position.x, position.y, position.z);
    }
}

void PlayScene::render() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    Scene::render();
}
