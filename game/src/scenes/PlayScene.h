/**
 * @file    PlayScene.h
 * @brief   PlayScene class for managing the gameplay scene.
 * @details The PlayScene class is responsible for managing the gameplay of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-02
 */

#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include "core/Scene.h"
#include "graphics/cameras/IsometricCamera.h"
#include "graphics/meshes/Cube.h"
#include "graphics/meshes/Sphere.h"
#include "graphics/renderers/ShaderProgram.h"

/**
 * @class PlayScene
 * @brief Represents the gameplay scene of the game.
 */
class PlayScene final : public Scene {
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


#endif //PLAYSCENE_H
