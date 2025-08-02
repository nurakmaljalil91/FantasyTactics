/**
 * @file    PlayScene.cpp
 * @brief   PlayScene class for managing the gameplay scene.
 * @details The PlayScene class is responsible for managing the gameplay of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-02
 */
#include "PlayScene.h"

#include "glm/gtc/type_ptr.inl"

void PlayScene::initialize() {
    _shader.loadShaders("resources/shaders/default.vert", "resources/shaders/default.frag");
    _texture.loadTexture("resources/textures/crate.jpg", true);
    _robotTexture.loadTexture("resources/textures/robot_diffuse.jpg", true);
    if (!_robotMesh.loadObj("resources/models/robot.obj")) {
        Logger::log()->error("Failed to load robot mesh");
    } else {
        Logger::log()->info("Robot mesh loaded successfully");
    }

    // Construct your isometric camera
    // Suppose you want an ortho box from -2..2 horizontally, -2..2 vertically
    // near=0.1, far=100.0
    // Instead of (-2,2), try something bigger:


    // glfwSetWindowUserPointer(glfwWindow, &glfwWindow);
    // glfwSetWindowUserPointer(glfwWindow, &isometricCamera);
    // glfwSetScrollCallback(glfwWindow, IsometricCamera::scrollCallback);
}


void PlayScene::update(float deltaTime) {
    _isometricCamera.updateCamera();
}

void PlayScene::render() {
    // specify the color of the background
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the Cel shader
    _shader.use();

    GLint useTexLocation = glGetUniformLocation(_shader.getProgram(), "uUseTexture");

    // Bind the crate texture to texture unit 0
    _texture.bind(0);

    // Tell the fragment shader that diffuseTexture is in GL_TEXTURE0
    const GLuint diffuseLocation = glGetUniformLocation(_shader.getProgram(), "diffuseTexture");
    glUniform1i(static_cast<int>(diffuseLocation), 0);
    glUniform1f(useTexLocation, 1.0f); // Use texture

    // Build model/view/projection
    auto model = glm::mat4(1.0f);

    model = glm::translate(model, _cubePosition);
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

    // Draw the sphere
    _cube.draw();

    // disable texturing
    glUniform1i(useTexLocation, 0);
    // set flat white (or any color you like)
    glUniform3f(static_cast<GLint>(baseColorLocation), 1.0f, 1.0f, 1.0f);

    model = glm::mat4(1.0f); // reset model matrix
    model = glm::translate(model, _spherePosition);
    glUniformMatrix4fv(static_cast<int>(modelLocation), 1, GL_FALSE, glm::value_ptr(model));
    _sphere.draw();

    // Bind the crate texture to texture unit 0
    _robotTexture.bind(0);

    // Tell the fragment shader that diffuseTexture is in GL_TEXTURE0
    const GLuint robotDiffuseLocation = glGetUniformLocation(_shader.getProgram(), "diffuseTexture");
    glUniform1i(static_cast<int>(robotDiffuseLocation), 0);
    glUniform1f(useTexLocation, 1.0f); // Use texture

    // Render the robot mesh
    model = glm::mat4(1.0f); // reset model matrix
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(0.5f)); // Scale down the robot

    // pass matrices to the shader
    glUniformMatrix4fv(static_cast<int>(modelLocation), 1, GL_FALSE, glm::value_ptr(model));
    _robotMesh.draw();
}
