/**
 * @file    TestScene.h
 * @brief   TestScene class for testing purposes.
 * @details The TestScene class is a placeholder for testing various features and functionalities
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-21
 */

#ifndef FANTASYTACTICS_TESTSCENE_H
#define FANTASYTACTICS_TESTSCENE_H

#include "core/Scene.h"
#include "graphics/cameras/IsometricCamera.h"
#include "graphics/meshes/Cube.h"
#include "graphics/meshes/Sphere.h"
#include "graphics/renderers/ShaderProgram.h"

/**
 * @class TestScene
 * @brief Placeholder class for testing purposes.
 */
class TestScene final : public cbit::Scene {
public:
    void initialize() override;

    void update(float deltaTime) override;

    void render() override;

private:
    ShaderProgram _shader;
    IsometricCamera _isometricCamera{
        {0.0f, 0.0f, 0.0f}, 5.0f, 5.0f
    };
    Cube _cube;
    Sphere _sphere;
    glm::vec3 _cubePosition{0.0f, 0.0f, 0.0f};
    glm::vec3 _spherePosition{0.0f, 1.5f, 0.0f};
    Texture _texture;
    Texture _robotTexture;
    Mesh _robotMesh;
};


#endif //FANTASYTACTICS_TESTSCENE_H
