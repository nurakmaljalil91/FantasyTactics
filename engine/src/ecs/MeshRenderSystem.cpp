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
#include "utilities/Logger.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

cbit::MeshRenderSystem::MeshRenderSystem(entt::registry &registry) : _registry(registry) {
    // Constructor implementation can be added here if needed
    _shader.loadShaders("resources/shaders/default.vert", "resources/shaders/default.frag");
}

void cbit::MeshRenderSystem::setWindow(GLFWwindow *window) {
    _window = window;
}

cbit::ShaderProgram *cbit::MeshRenderSystem::_getShader(const std::string &vertexPath, const std::string &fragmentPath) {
    if (vertexPath.empty() || fragmentPath.empty()) {
        return &_shader;
    }

    const std::string key = vertexPath + "|" + fragmentPath;
    const auto found = _shaderCache.find(key);
    if (found != _shaderCache.end()) {
        return found->second.get();
    }

    auto shader = std::make_unique<ShaderProgram>();
    if (!shader->loadShaders(vertexPath.c_str(), fragmentPath.c_str())) {
        Logger::log()->error("Failed to load shader override: {} / {}", vertexPath, fragmentPath);
        return &_shader;
    }

    auto *shaderPtr = shader.get();
    _shaderCache.emplace(key, std::move(shader));
    return shaderPtr;
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

    glm::vec3 lightDirection{-10.0f, -10.0f, -1.0f};
    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
    float ambientStrength = 0.25f;
    float lightIntensity = 1.0f;

    auto lightView = _registry.view<DirectionalLightComponent, TransformComponent>();
    if (lightView.begin() != lightView.end()) {
        auto &lightComponent = lightView.get<DirectionalLightComponent>(*lightView.begin());
        if (lightComponent.enabled) {
            lightDirection = lightComponent.direction.toGLM();
            lightColor = lightComponent.diffuse.toGLM();
            ambientStrength = (lightComponent.ambient.x + lightComponent.ambient.y + lightComponent.ambient.z) / 3.0f;
            lightIntensity = lightComponent.intensity;
        }
    }

    const auto cubeView = _registry.view<CubeComponent, TransformComponent>();
    ShaderProgram *currentShader = nullptr;

    for (const auto entity: cubeView) {
        auto [cube, transform] = cubeView.get<CubeComponent, TransformComponent>(entity);
        const auto *shaderOverride = _registry.try_get<ShaderOverrideComponent>(entity);
        const std::string vertexPath = shaderOverride ? shaderOverride->vertexShaderPath : "";
        const std::string fragmentPath = shaderOverride ? shaderOverride->fragmentShaderPath : "";
        ShaderProgram *shader = _getShader(vertexPath, fragmentPath);

        if (shader != currentShader) {
            shader->use();

            if (shader->hasUniform("uView")) {
                shader->setUniform("uView", viewMatrix);
            }
            if (shader->hasUniform("uProjection")) {
                shader->setUniform("uProjection", projection);
            }
            if (shader->hasUniform("lightDir")) {
                shader->setUniform("lightDir", lightDirection);
            }
            if (shader->hasUniform("lightColor")) {
                shader->setUniform("lightColor", lightColor);
            }
            if (shader->hasUniform("baseColor")) {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
            if (shader->hasUniform("uUseTexture")) {
                shader->setUniform("uUseTexture", 0);
            }
            if (shader->hasUniform("ambientStrength")) {
                shader->setUniform("ambientStrength", ambientStrength);
            }
            if (shader->hasUniform("lightIntensity")) {
                shader->setUniform("lightIntensity", lightIntensity);
            }
            if (shader->hasUniform("lightWrap")) {
                shader->setUniform("lightWrap", 0.35f);
            }

            currentShader = shader;
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position.toGLM());
        if (currentShader && currentShader->hasUniform("uModel")) {
            currentShader->setUniform("uModel", model);
        }

        cube.cube.draw();
    }
}
