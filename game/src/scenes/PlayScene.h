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

/**
 * @class PlayScene
 * @brief Represents the gameplay scene of the game.
 */
class PlayScene final : public Scene {
public:
    void initialize() override;

    void update(float deltaTime) override;

    void render() const override;
};


#endif //PLAYSCENE_H
