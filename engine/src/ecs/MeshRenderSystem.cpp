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

cbit::MeshRenderSystem::MeshRenderSystem(entt::registry &registry) : _registry(registry) {
    // Constructor implementation can be added here if needed
    _shader.loadShaders("resources/shaders/default.vert", "resources/shaders/default.frag");
}

void cbit::MeshRenderSystem::render(Camera &camera, int windowWidth, int windowHeight) {
    auto view = _registry.view<CubeComponent, TransformComponent>();

    _shader.use();
    float aspectRatio = static_cast<float>(windowWidth) / windowHeight;
    glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
    glm::mat4 viewMatrix = camera.getViewMatrix();

    _shader.setUniform("uView", viewMatrix);
    _shader.setUniform("uProjection", projection);

    _shader.setUniform("lightDir", glm::vec3(-10.f, -10.0f, -1.0f));
    _shader.setUniform("baseColor", glm::vec3(0.8f, 0.8f, 0.1f));

    for (auto entity: view) {
        auto [cube, transform] = view.get<CubeComponent, TransformComponent>(entity);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position.getGlmVector());
        _shader.setUniform("uModel", model);

        cube.cube.draw();
    }
}
