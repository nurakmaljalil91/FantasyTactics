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

/**
 * @class PlayScene
 * @brief Represents the gameplay scene of the game.
 */
class PlayScene final : public cbit::Scene {
public:
    void initialize() override;

    void update(float deltaTime) override;

    void render() override;

private:
    ShaderProgram _shader;
    cbit::IsometricCamera _isometricCamera{
            {0.0f, 0.0f, 0.0f}, 5.0f, 5.0f
        };
};


#endif //PLAYSCENE_H
