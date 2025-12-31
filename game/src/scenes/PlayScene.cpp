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
#include "math/Color.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "entt/entt.hpp"
#include <cmath>
#include <string>

#include "../systems/DebugTransformSystem.h"
#include "../systems/GridMovementSystem.h"

namespace {
    constexpr int kGridWidth = 8;
    constexpr int kGridHeight = 8;
    constexpr float kTileSize = 1.0f;
    constexpr float kTileHeight = 1.0f;

    const int kHeightMap[kGridHeight][kGridWidth] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1}
    };

    cbit::Vector3 gridToWorldTop(const int x, const int z) {
        constexpr float originX = -((kGridWidth - 1) * kTileSize) * 0.5f;
        constexpr float originZ = -((kGridHeight - 1) * kTileSize) * 0.5f;

        const int height = kHeightMap[z][x];
        return {
            originX + static_cast<float>(x) * kTileSize,
            (static_cast<float>(height) + 0.5f) * kTileHeight,
            originZ + static_cast<float>(z) * kTileSize
        };
    }

    bool isGridInBounds(const int x, const int z) {
        return x >= 0 && x < kGridWidth && z >= 0 && z < kGridHeight;
    }

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
    getWorld().addSystem<GridMovementSystem>(
        [](int x, int z) { return gridToWorldTop(x, z); }, kGridWidth, kGridHeight);
    getWorld().addSystem<DebugTransformSystem>();

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
            .addComponent<GridMovementComponent>()
            .addComponent<cbit::MeshComponent>("assets/models/characterMedium.fbx")
            .addComponent<cbit::TextureComponent>("assets/textures/skaterMaleA.png");

    auto &[playerPosition, playerRotation, playerScale] = player.getComponent<cbit::TransformComponent>();
    auto &playerGrid = player.getComponent<GridMovementComponent>();
    playerGrid.x = 2;
    playerGrid.z = 2;
    playerPosition = gridToWorldTop(playerGrid.x, playerGrid.z);
    playerRotation = cbit::Vector3{-260.0f, -190.0f, 0.0f};
    playerScale = cbit::Vector3{0.6f, 0.6f, 0.6f};

    constexpr float axisLength = 0.6f;
    constexpr float axisThickness = 0.04f;

    auto addAxisGizmo = [&](const std::string &name,
                            const cbit::Vector3 &offset,
                            const cbit::Vector3 &scale,
                            const cbit::Vector3 &color) {
        auto axis = getWorld().createGameObject(name)
                .addComponent<cbit::TransformComponent>()
                .addComponent<DebugFollowComponent>()
                .addComponent<cbit::CubeComponent>()
                .addComponent<cbit::BaseColorComponent>();

        auto &follow = axis.getComponent<DebugFollowComponent>();
        follow.target = player.getEntity();
        follow.positionOffset = offset;
        follow.scale = scale;
        follow.followRotation = true;
        follow.followScale = false;
        follow.offsetInLocalSpace = true;

        auto &baseColor = axis.getComponent<cbit::BaseColorComponent>();
        baseColor.color = color;
    };

    addAxisGizmo(
        "player_axis_x",
        cbit::Vector3{axisLength * 0.5f, 0.0f, 0.0f},
        cbit::Vector3{axisLength, axisThickness, axisThickness},
        cbit::Vector3{1.0f, 0.0f, 0.0f});
    addAxisGizmo(
        "player_axis_y",
        cbit::Vector3{0.0f, axisLength * 0.5f, 0.0f},
        cbit::Vector3{axisThickness, axisLength, axisThickness},
        cbit::Vector3{0.0f, 1.0f, 0.0f});
    addAxisGizmo(
        "player_axis_z",
        cbit::Vector3{0.0f, 0.0f, axisLength * 0.5f},
        cbit::Vector3{axisThickness, axisThickness, axisLength},
        cbit::Vector3{0.0f, 0.0f, 1.0f});

    getWorld().createGameObject("player_transform_text")
            .addComponent<cbit::UIAnchorComponent>(
                cbit::UIAnchor::TopCenter,
                cbit::Vector2(12.0f, -12.0f),
                cbit::Vector2(320.0f, 90.0f),
                50, true, false)
            .addComponent<cbit::UITextComponent>("Transform:", 0.3f, cbit::Color::Black)
            .addComponent<DebugTransformTextComponent>(DebugTransformTextComponent{player.getEntity()});

    auto enemy = getWorld().createGameObject("enemy")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::MeshComponent>("assets/models/characterMedium.fbx")
            .addComponent<cbit::TextureComponent>("assets/textures/criminalMaleA.png");

    auto &[enemyPosition, enemyRotation, enemyScale] = enemy.getComponent<cbit::TransformComponent>();
    auto &enemyGrid = enemy.getComponent<cbit::TransformComponent>();
    constexpr auto enemyGridX = 5;
    constexpr auto enemyGridZ = 2;
    enemyPosition = gridToWorldTop(enemyGridX, enemyGridZ);
    enemyRotation = cbit::Vector3{-260.0f, -190.0f, 0.0f};
    enemyScale = cbit::Vector3{0.6f, 0.6f, 0.6f};
}


void PlayScene::update(const float deltaTime) {
    static bool useCelShader = false;
    if (cbit::Input::isKeyPressed(cbit::Keyboard::C)) {
        useCelShader = !useCelShader;
        applyShaderOverrideToTiles(getWorld(), useCelShader);
    }

    Scene::update(deltaTime);

    auto player = getWorld().getGameObject("player");

    if (player.getEntity() != entt::null) {
        auto &transform = player.getComponent<cbit::TransformComponent>();

        if (cbit::Input::isKeyDown(cbit::Keyboard::Up)) {
            transform.rotation.x -= 10.0f * deltaTime;
        } else if (cbit::Input::isKeyDown(cbit::Keyboard::Down)) {
            transform.rotation.x += 10.0f * deltaTime;
        } else if (cbit::Input::isKeyDown(cbit::Keyboard::Left)) {
            transform.rotation.z += 10.0f * deltaTime;
        } else if (cbit::Input::isKeyDown(cbit::Keyboard::Right)) {
            transform.rotation.z -= 10.0f * deltaTime;
        }
    }

}

void PlayScene::render() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    Scene::render();
}
