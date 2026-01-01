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
#include <algorithm>
#include <vector>
#include "utilities/Logger.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

cbit::MeshRenderSystem::MeshRenderSystem(entt::registry &registry) : _registry(registry) {
    // Constructor implementation can be added here if needed
    _shader.loadShaders("resources/shaders/default.vert", "resources/shaders/default.frag");
    _skinnedShader.loadShaders("resources/shaders/skinned.vert", "resources/shaders/default.frag");
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

    ShaderProgram *currentShader = nullptr;

    auto applyShaderForEntity = [&](entt::entity entity, ShaderProgram *fallbackShader) -> ShaderProgram * {
        const auto *shaderOverride = _registry.try_get<ShaderOverrideComponent>(entity);
        const std::string vertexPath = shaderOverride ? shaderOverride->vertexShaderPath : "";
        const std::string fragmentPath = shaderOverride ? shaderOverride->fragmentShaderPath : "";
        ShaderProgram *shader = fallbackShader;

        if (!vertexPath.empty() && !fragmentPath.empty()) {
            shader = _getShader(vertexPath, fragmentPath);
        }

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

        return shader;
    };

    auto applyTextureForEntity = [&](ShaderProgram *shader, entt::entity entity) {
        const auto *textureComponent = _registry.try_get<TextureComponent>(entity);
        const bool hasTexture = textureComponent && !textureComponent->path.empty();

        if (shader->hasUniform("uUseTexture")) {
            shader->setUniform("uUseTexture", hasTexture ? 1 : 0);
        }

        if (hasTexture && shader->hasUniform("diffuseTexture")) {
            auto [it, inserted] = _textures.try_emplace(textureComponent->path);
            if (inserted) {
                it->second.loadTexture(textureComponent->path);
            }
            glActiveTexture(GL_TEXTURE0);
            it->second.bind();
            shader->setUniform("diffuseTexture", 0);
        }
    };

    auto buildModelMatrix = [](const TransformComponent &transform) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position.toGLM());
        const glm::vec3 rotation = transform.rotation.toGLM();
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, transform.scale.toGLM());
        return model;
    };

    const auto skyView = _registry.view<SkyboxComponent, QuadComponent, TransformComponent, TextureComponent>();
    if (skyView.begin() != skyView.end()) {
        const glm::mat4 invViewRot = glm::mat4(glm::transpose(glm::mat3(viewMatrix)));
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);

        for (const auto entity: skyView) {
            auto [skybox, quad, transform] = skyView.get<SkyboxComponent, QuadComponent, TransformComponent>(entity);
            ShaderProgram *shader = applyShaderForEntity(entity, &_shader);

            if (shader->hasUniform("uInvViewRot")) {
                shader->setUniform("uInvViewRot", invViewRot);
            }

            applyTextureForEntity(shader, entity);
            quad.quad.draw();
        }

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
    }

    const auto cubeView = _registry.view<CubeComponent, TransformComponent>();
    for (const auto entity: cubeView) {
        auto [cube, transform] = cubeView.get<CubeComponent, TransformComponent>(entity);
        ShaderProgram *shader = applyShaderForEntity(entity, &_shader);

        glm::mat4 model = buildModelMatrix(transform);
        if (shader->hasUniform("uModel")) {
            shader->setUniform("uModel", model);
        }

        if (shader->hasUniform("baseColor")) {
            const auto *baseColor = _registry.try_get<BaseColorComponent>(entity);
            if (baseColor) {
                shader->setUniform("baseColor", baseColor->color.toGLM());
            } else {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
        }

        applyTextureForEntity(shader, entity);
        cube.cube.draw();
    }

    const auto circleView = _registry.view<CircleComponent, TransformComponent>();
    for (const auto entity: circleView) {
        auto [circle, transform] = circleView.get<CircleComponent, TransformComponent>(entity);
        ShaderProgram *shader = applyShaderForEntity(entity, &_shader);

        glm::mat4 model = buildModelMatrix(transform);
        if (shader->hasUniform("uModel")) {
            shader->setUniform("uModel", model);
        }

        if (shader->hasUniform("baseColor")) {
            const auto *baseColor = _registry.try_get<BaseColorComponent>(entity);
            if (baseColor) {
                shader->setUniform("baseColor", baseColor->color.toGLM());
            } else {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
        }

        applyTextureForEntity(shader, entity);
        circle.circle.draw();
    }

    const auto quadView = _registry.view<QuadComponent, TransformComponent>();
    for (const auto entity: quadView) {
        auto [quad, transform] = quadView.get<QuadComponent, TransformComponent>(entity);
        ShaderProgram *shader = applyShaderForEntity(entity, &_shader);

        glm::mat4 model = buildModelMatrix(transform);
        if (shader->hasUniform("uModel")) {
            shader->setUniform("uModel", model);
        }

        if (shader->hasUniform("baseColor")) {
            const auto *baseColor = _registry.try_get<BaseColorComponent>(entity);
            if (baseColor) {
                shader->setUniform("baseColor", baseColor->color.toGLM());
            } else {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
        }

        applyTextureForEntity(shader, entity);
        quad.quad.draw();
    }

    const auto sphereView = _registry.view<SphereComponent, TransformComponent>();
    for (const auto entity: sphereView) {
        auto [sphere, transform] = sphereView.get<SphereComponent, TransformComponent>(entity);
        ShaderProgram *shader = applyShaderForEntity(entity, &_shader);

        glm::mat4 model = buildModelMatrix(transform);
        if (shader->hasUniform("uModel")) {
            shader->setUniform("uModel", model);
        }

        if (shader->hasUniform("baseColor")) {
            const auto *baseColor = _registry.try_get<BaseColorComponent>(entity);
            if (baseColor) {
                shader->setUniform("baseColor", baseColor->color.toGLM());
            } else {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
        }

        applyTextureForEntity(shader, entity);
        sphere.sphere.draw();
    }

    const auto ellipsoidView = _registry.view<EllipsoidComponent, TransformComponent>();
    for (const auto entity: ellipsoidView) {
        auto [ellipsoid, transform] = ellipsoidView.get<EllipsoidComponent, TransformComponent>(entity);
        ShaderProgram *shader = applyShaderForEntity(entity, &_shader);

        glm::mat4 model = buildModelMatrix(transform);
        if (shader->hasUniform("uModel")) {
            shader->setUniform("uModel", model);
        }

        if (shader->hasUniform("baseColor")) {
            const auto *baseColor = _registry.try_get<BaseColorComponent>(entity);
            if (baseColor) {
                shader->setUniform("baseColor", baseColor->color.toGLM());
            } else {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
        }

        applyTextureForEntity(shader, entity);
        ellipsoid.ellipsoid.draw();
    }

    const auto meshView = _registry.view<MeshComponent, TransformComponent>();
    for (const auto entity: meshView) {
        auto [meshComponent, transform] = meshView.get<MeshComponent, TransformComponent>(entity);
        ShaderProgram *shader = applyShaderForEntity(entity, &_shader);

        glm::mat4 model = buildModelMatrix(transform);
        if (shader->hasUniform("uModel")) {
            shader->setUniform("uModel", model);
        }

        if (shader->hasUniform("baseColor")) {
            const auto *baseColor = _registry.try_get<BaseColorComponent>(entity);
            if (baseColor) {
                shader->setUniform("baseColor", baseColor->color.toGLM());
            } else {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
        }

        applyTextureForEntity(shader, entity);
        meshComponent.mesh.draw();
    }

    const auto skinnedView = _registry.view<SkinnedMeshComponent, TransformComponent>();
    for (const auto entity: skinnedView) {
        auto [skinnedMesh, transform] = skinnedView.get<SkinnedMeshComponent, TransformComponent>(entity);
        const auto *animatorComponent = _registry.try_get<AnimatorComponent>(entity);
        const bool useSkinning = animatorComponent && !animatorComponent->activeClip.empty();
        ShaderProgram *shader = applyShaderForEntity(entity, useSkinning ? &_skinnedShader : &_shader);

        Logger::log()->info("Skinned render entity {} vertices={} indices={} bones={} useSkinning={}",
                            static_cast<int>(entity),
                            skinnedMesh.mesh.getVertexCount(),
                            skinnedMesh.mesh.getIndexCount(),
                            skinnedMesh.mesh.getBoneCount(),
                            useSkinning ? 1 : 0);

        glm::mat4 model = buildModelMatrix(transform);
        if (shader->hasUniform("uModel")) {
            shader->setUniform("uModel", model);
        }

        if (shader->hasUniform("baseColor")) {
            const auto *baseColor = _registry.try_get<BaseColorComponent>(entity);
            if (baseColor) {
                shader->setUniform("baseColor", baseColor->color.toGLM());
            } else {
                shader->setUniform("baseColor", glm::vec3(0.83f, 0.83f, 0.83f));
            }
        }

        applyTextureForEntity(shader, entity);

        if (shader->hasUniform("uUseSkinning")) {
            shader->setUniform("uUseSkinning", useSkinning ? 1 : 0);
        }

        const size_t boneCount = skinnedMesh.mesh.getBoneCount();
        if (boneCount > 0 && shader->hasUniform("uBones[0]")) {
            const auto *animatorComponent = _registry.try_get<AnimatorComponent>(entity);
            const auto *boneMatrices = animatorComponent ? &animatorComponent->animator.getFinalBoneMatrices() : nullptr;
            std::vector<glm::mat4> identityMatrices;

            if (!boneMatrices || boneMatrices->size() < boneCount) {
                identityMatrices.assign(boneCount, glm::mat4(1.0f));
                boneMatrices = &identityMatrices;
            }

            const size_t maxCount = std::min(boneCount, boneMatrices->size());
            for (size_t i = 0; i < maxCount; ++i) {
                const std::string uniformName = "uBones[" + std::to_string(i) + "]";
                shader->setUniform(uniformName.c_str(), (*boneMatrices)[i]);
            }
        }

        skinnedMesh.mesh.draw();
    }
}
