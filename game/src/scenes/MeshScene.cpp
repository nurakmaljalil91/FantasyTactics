/**
 * @file    MeshScene.cpp
 * @brief   Mesh scene for rendering various 3D meshes.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-31
 */

#include "MeshScene.h"

#include "ecs/Components.h"
#include "ecs/GameObject.h"
#include "math/Color.h"

namespace {
    void setupIsometricCamera(cbit::EntityComponentSystem &world) {
        auto mainCamera = world.createGameObject("MeshSceneCamera")
                .addComponent<cbit::TransformComponent>()
                .addComponent<cbit::CameraComponent>()
                .addComponent<cbit::ActiveCameraComponent>();

        auto &cameraComponent = mainCamera.getComponent<cbit::CameraComponent>();
        cameraComponent.type = cbit::CameraType::Isometric;
        cameraComponent.yaw = 45.0f;
        cameraComponent.pitch = -35.264f;
        cameraComponent.orthoLeft = -6.0f;
        cameraComponent.orthoRight = 6.0f;
        cameraComponent.orthoBottom = -6.0f;
        cameraComponent.orthoTop = 6.0f;

        auto &cameraTransform = mainCamera.getComponent<cbit::TransformComponent>();
        cameraTransform.position = cbit::Vector3{-8.0f, 8.0f, -8.0f};
    }

    void setupLight(cbit::EntityComponentSystem &world) {
        auto mainLight = world.createGameObject("MeshSceneLight")
                .addComponent<cbit::TransformComponent>()
                .addComponent<cbit::DirectionalLightComponent>();

        auto &lightComponent = mainLight.getComponent<cbit::DirectionalLightComponent>();
        lightComponent.direction = cbit::Vector3{-6.0f, -1.0f, -1.0f};
        lightComponent.ambient = cbit::Vector3{0.55f, 0.55f, 0.55f};
        lightComponent.diffuse = cbit::Vector3{1.0f, 1.0f, 1.0f};
        lightComponent.intensity = 1.0f;
    }
}

void MeshScene::initialize() {
    setBackgroundColor(cbit::Color{0.53f, 0.81f, 0.92f, 1.0f});
    setupIsometricCamera(getWorld());
    setupLight(getWorld());

    auto cube = getWorld().createGameObject("mesh_cube")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::CubeComponent>()
            .addComponent<cbit::BaseColorComponent>();
    cube.getComponent<cbit::TransformComponent>().position = cbit::Vector3{-2.5f, 0.5f, 0.0f};
    cube.getComponent<cbit::BaseColorComponent>().color = cbit::Vector3{0.9f, 0.2f, 0.2f};

    auto sphere = getWorld().createGameObject("mesh_sphere")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::SphereComponent>(0.7f, 16, 32)
            .addComponent<cbit::BaseColorComponent>();
    sphere.getComponent<cbit::TransformComponent>().position = cbit::Vector3{0.0f, 0.7f, 0.0f};
    sphere.getComponent<cbit::BaseColorComponent>().color = cbit::Vector3{0.2f, 0.7f, 0.9f};

    auto ellipsoid = getWorld().createGameObject("mesh_ellipsoid")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::EllipsoidComponent>(0.8f, 0.5f, 1.1f, 36, 18)
            .addComponent<cbit::BaseColorComponent>();
    ellipsoid.getComponent<cbit::TransformComponent>().position = cbit::Vector3{2.5f, 0.6f, 0.0f};
    ellipsoid.getComponent<cbit::BaseColorComponent>().color = cbit::Vector3{0.9f, 0.6f, 0.2f};

    auto circle = getWorld().createGameObject("mesh_circle")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::CircleComponent>(0.7f, 32)
            .addComponent<cbit::BaseColorComponent>();
    auto &circleTransform = circle.getComponent<cbit::TransformComponent>();
    circleTransform.position = cbit::Vector3{-1.5f, 0.01f, -2.5f};
    circleTransform.rotation = cbit::Vector3{-90.0f, 0.0f, 0.0f};
    circle.getComponent<cbit::BaseColorComponent>().color = cbit::Vector3{0.2f, 0.9f, 0.4f};

    auto quad = getWorld().createGameObject("mesh_quad")
            .addComponent<cbit::TransformComponent>()
            .addComponent<cbit::QuadComponent>()
            .addComponent<cbit::BaseColorComponent>();
    auto &quadTransform = quad.getComponent<cbit::TransformComponent>();
    quadTransform.position = cbit::Vector3{1.5f, 0.01f, -2.5f};
    quadTransform.rotation = cbit::Vector3{-90.0f, 0.0f, 0.0f};
    quadTransform.scale = cbit::Vector3{1.2f, 1.2f, 1.2f};
    quad.getComponent<cbit::BaseColorComponent>().color = cbit::Vector3{0.7f, 0.3f, 0.9f};
}

void MeshScene::update(float deltaTime) {
    Scene::update(deltaTime);
}

void MeshScene::render() {
    Scene::render();
}
