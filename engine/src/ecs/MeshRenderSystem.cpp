/**
 * @file    MeshRenderSystem.cpp
 * @brief   Implementation of the MeshRenderSystem class.
 * @details This file contains the implementation of the MeshRenderSystem class,
 *          which is responsible for rendering mesh components in the entity-component system.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */
#include "MeshRenderSystem.h"

#include "Components.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

cbit::MeshRenderSystem::MeshRenderSystem(entt::registry &registry) : _registry(registry) {
    // Constructor implementation can be added here if needed
    _shader.loadShaders("resources/shaders/default.vert", "resources/shaders/default.frag");
}

void cbit::MeshRenderSystem::setWindow(GLFWwindow *window) {
    _window = window;
}

void cbit::MeshRenderSystem::render() {
    const auto cameraView = _registry.view<CameraComponent, TransformComponent, ActiveCameraComponent>();

    if (cameraView.begin() == cameraView.end()) {
        return;
    }

    const auto cameraEntity = *cameraView.begin();

    const auto &cameraComponent = cameraView.get<CameraComponent>(cameraEntity);

    const auto &cameraTransformComponent = cameraView.get<TransformComponent>(cameraEntity);

    if (!_window) {
        return;
    }

    int windowWidth, windowHeight;

    glfwGetFramebufferSize(_window, &windowWidth, &windowHeight);

    if (windowWidth <= 0 || windowHeight <= 0) {
        return;
    }

    _shader.use();

    const float aspectRatio = static_cast<float>(windowWidth) / windowHeight;

    glm::mat4 projection;

    switch (cameraComponent.type) {
        case CameraType::Perspective:
            projection = glm::perspective(
                glm::radians(cameraComponent.fov),
                aspectRatio,
                cameraComponent.nearPlane,
                cameraComponent.farPlane);
            break;
        case CameraType::Orthographic:
            projection = glm::ortho(
                cameraComponent.orthoLeft,
                cameraComponent.orthoRight,
                cameraComponent.orthoBottom,
                cameraComponent.orthoTop,
                cameraComponent.nearPlane,
                cameraComponent.farPlane);
            break;
        case CameraType::Isometric: {
            projection = glm::ortho(
                cameraComponent.orthoLeft,
                cameraComponent.orthoRight,
                cameraComponent.orthoBottom,
                cameraComponent.orthoTop,
                cameraComponent.nearPlane,
                cameraComponent.farPlane);
            break;
        }
        default:
            throw std::runtime_error("Unknown camera type");
    }

    const float yaw = glm::radians(cameraComponent.yaw);
    const float pitch = glm::radians(cameraComponent.pitch);

    // Standard FPS-style forward vector
    glm::vec3 forward;
    forward.x = cosf(pitch) * cosf(yaw);
    forward.y = sinf(pitch);
    forward.z = cosf(pitch) * sinf(yaw);


    const glm::vec3 cameraPosition = cameraTransformComponent.position.toGLM();
    const glm::vec3 cameraTarget = cameraPosition + glm::normalize(forward);
    constexpr auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    const glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    _shader.setUniform("uView", viewMatrix);
    _shader.setUniform("uProjection", projection);

    glm::vec3 lightDirection{-10.0f, -10.0f, -1.0f};
    float ambientStrength = 0.25f;
    float lightIntensity = 1.0f;

    auto lightView = _registry.view<DirectionalLightComponent, TransformComponent>();
    if (lightView.begin() != lightView.end()) {
        auto &lightComponent = lightView.get<DirectionalLightComponent>(*lightView.begin());
        if (lightComponent.enabled) {
            lightDirection = lightComponent.direction.toGLM();
            ambientStrength = (lightComponent.ambient.x + lightComponent.ambient.y + lightComponent.ambient.z) / 3.0f;
            lightIntensity = lightComponent.intensity;
        }
    }

    _shader.setUniform("lightDir", lightDirection);
    _shader.setUniform("baseColor", glm::vec3(0.8f, 0.8f, 0.1f));
    _shader.setUniform("uUseTexture", 0); // false
    _shader.setUniform("ambientStrength", ambientStrength);
    _shader.setUniform("lightIntensity", lightIntensity);

    const auto cubeView = _registry.view<CubeComponent, TransformComponent>();

    for (const auto entity: cubeView) {
        auto [cube, transform] = cubeView.get<CubeComponent, TransformComponent>(entity);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position.toGLM());
        _shader.setUniform("uModel", model);

        cube.cube.draw();
    }
}
