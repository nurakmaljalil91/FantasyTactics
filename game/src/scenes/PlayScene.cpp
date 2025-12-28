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

    glm::mat4 buildCameraProjection(const cbit::CameraComponent &camera, float aspectRatio) {
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
                return glm::mat4(1.0f);
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
        double screenX,
        double screenY,
        int screenWidth,
        int screenHeight,
        const glm::mat4 &projection,
        const glm::mat4 &view,
        float planeY,
        glm::vec3 &outWorld) {
        if (screenWidth <= 0 || screenHeight <= 0) {
            return false;
        }

        const float ndcX = static_cast<float>((2.0 * screenX) / screenWidth - 1.0);
        const float ndcY = static_cast<float>(1.0 - (2.0 * screenY) / screenHeight);

        const glm::mat4 invViewProjection = glm::inverse(projection * view);
        glm::vec4 nearPoint = invViewProjection * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
        glm::vec4 farPoint = invViewProjection * glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

        if (nearPoint.w == 0.0f || farPoint.w == 0.0f) {
            return false;
        }

        nearPoint /= nearPoint.w;
        farPoint /= farPoint.w;

        const glm::vec3 rayOrigin = glm::vec3(nearPoint);
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

    auto &playerTransformComponent = player.getComponent<cbit::TransformComponent>();
    playerTransformComponent.position = cbit::Vector3{0.0f, 0.0f, 0.0f};
    playerTransformComponent.scale = cbit::Vector3{0.1f, 0.1f, 0.1f};
}


void PlayScene::update(float deltaTime) {
    static bool useCelShader = false;
    if (cbit::Input::isKeyPressed(cbit::Keyboard::C)) {
        useCelShader = !useCelShader;
        applyShaderOverrideToTiles(getWorld(), useCelShader);
    }

    auto mainLighting = getWorld().getGameObject("MainLighting");
    auto &lightComponent = mainLighting.getComponent<cbit::DirectionalLightComponent>();

    constexpr float directionSpeed = 2.0f;
    if (cbit::Input::isKeyDown(cbit::Keyboard::A)) {
        lightComponent.direction.x -= directionSpeed * deltaTime;
        cbit::Logger::log()->info("light direction ({}, {}, {})", lightComponent.direction.x, lightComponent.direction.y, lightComponent.direction.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::D)) {
        lightComponent.direction.x += directionSpeed * deltaTime;
        cbit::Logger::log()->info("light direction ({}, {}, {})", lightComponent.direction.x, lightComponent.direction.y, lightComponent.direction.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::W)) {
        lightComponent.direction.z -= directionSpeed * deltaTime;
        cbit::Logger::log()->info("light direction ({}, {}, {})", lightComponent.direction.x, lightComponent.direction.y, lightComponent.direction.z);
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::S)) {
        lightComponent.direction.z += directionSpeed * deltaTime;
        cbit::Logger::log()->info("light direction ({}, {}, {})", lightComponent.direction.x, lightComponent.direction.y, lightComponent.direction.z);
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

    auto player = getWorld().getGameObject("player");

    auto &playerTransformComponent = player.getComponent<cbit::TransformComponent>();

    if (cbit::Input::isMouseDown(cbit::MouseButton::Left)) {
        double mouseX, mouseY;
        cbit::Input::getMousePosition(mouseX, mouseY);
        cbit::Logger::log()->info("Mouse Position: ({}, {})", mouseX, mouseY);

        const auto cameraView = getWorld().getRegistry().view<cbit::CameraComponent, cbit::TransformComponent, cbit::ActiveCameraComponent>();
        if (cameraView.begin() == cameraView.end()) {
            const auto cameraEntity = *cameraView.begin();
            const auto &cameraComponent = cameraView.get<cbit::CameraComponent>(cameraEntity);
            const auto &cameraTransform = cameraView.get<cbit::TransformComponent>(cameraEntity);

            const int width = getWindowWidth();
            const int height = getWindowHeight();
            const float aspectRatio = height > 0 ? static_cast<float>(width) / height : 1.0f;

            const glm::mat4 projection = buildCameraProjection(cameraComponent, aspectRatio);
            const glm::mat4 view = buildCameraView(cameraComponent, cameraTransform);

            glm::vec3 hitPoint{};
            if (screenToWorldOnPlane(mouseX, mouseY, width, height, projection, view, 0.0f, hitPoint)) {
                playerTransformComponent.position = cbit::Vector3{hitPoint.x, hitPoint.y, hitPoint.z};
            }
        }
    }

    constexpr float rotateSpeed = 90.0f;
    if (cbit::Input::isKeyDown(cbit::Keyboard::Z)) {
        playerTransformComponent.rotation.y -= rotateSpeed * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::X)) {
        playerTransformComponent.rotation.y += rotateSpeed * deltaTime;
    }

    if (cbit::Input::isKeyDown(cbit::Keyboard::N)) {
        playerTransformComponent.rotation.x -= rotateSpeed * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::M)) {
        playerTransformComponent.rotation.x += rotateSpeed * deltaTime;
    }

    if (cbit::Input::isKeyDown(cbit::Keyboard::U)) {
        playerTransformComponent.rotation.z -= rotateSpeed * deltaTime;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::J)) {
        playerTransformComponent.rotation.z += rotateSpeed * deltaTime;
    }

    constexpr float scaleStep = 0.05f;
    if (cbit::Input::isKeyDown(cbit::Keyboard::P)) {
        playerTransformComponent.scale.x += scaleStep;
        playerTransformComponent.scale.y += scaleStep;
        playerTransformComponent.scale.z += scaleStep;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::L)) {
        playerTransformComponent.scale.x = std::max(0.01f, playerTransformComponent.scale.x - scaleStep);
        playerTransformComponent.scale.y = std::max(0.01f, playerTransformComponent.scale.y - scaleStep);
        playerTransformComponent.scale.z = std::max(0.01f, playerTransformComponent.scale.z - scaleStep);
    }
}

void PlayScene::render() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    Scene::render();
}
