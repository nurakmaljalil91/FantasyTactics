/**
 * @file    PlayScene.cpp
 * @brief   PlayScene class for managing the gameplay scene.
 * @details The PlayScene class is responsible for managing the gameplay of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-02
 */
#include "PlayScene.h"

#include "ecs/GameObject.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"

void PlayScene::initialize() {
    _shader.loadShaders("resources/shaders/default.vert", "resources/shaders/default.frag");
    getWorld().createGameObject("Cube")
            .addComponent<TransformComponent>(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f)
            )
            .addComponent<CubeComponent>();
}


void PlayScene::update(float deltaTime) {
}

void PlayScene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto view = getWorld().getGameObjectsWith<CubeComponent, TransformComponent>();

    for (const auto entity: view) {
        auto [cube, transform] = view.get<CubeComponent, TransformComponent>(entity);

        _shader.use();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
        glm::mat4 view = _isometricCamera.getViewMatrix();
        float aspect = static_cast<float>(getWindowHeight()) / static_cast<float>(getWindowHeight());
        glm::mat4 projection = _isometricCamera.getProjectionMatrix(aspect);

        const GLuint modelLocation = glGetUniformLocation(_shader.getProgram(), "uModel");
        const GLuint viewLocation = glGetUniformLocation(_shader.getProgram(), "uView");
        const GLuint projectionLocation = glGetUniformLocation(_shader.getProgram(), "uProjection");

        glUniformMatrix4fv(static_cast<int>(modelLocation), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(static_cast<int>(viewLocation), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(static_cast<int>(projectionLocation), 1, GL_FALSE, glm::value_ptr(projection));

        // Cel shading lighting uniforms
        glm::vec3 lightDirection(-10.f, -10.0f, -1.0f);
        const GLuint lightDirectionLocation = glGetUniformLocation(_shader.getProgram(), "lightDir");
        glUniform3fv(static_cast<int>(lightDirectionLocation), 1, glm::value_ptr(lightDirection));

        glm::vec3 baseColor(0.8f, 0.8f, 0.1f);
        const GLuint baseColorLocation = glGetUniformLocation(_shader.getProgram(), "baseColor");
        glUniform3fv(static_cast<int>(baseColorLocation), 1, glm::value_ptr(baseColor));

        cube.cube.draw();
    }
}
