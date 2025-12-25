/**
 * @file    MenuScene.h
 * @brief   MenuScene class for managing the main menu scene.
 * @details The MenuScene class is responsible for managing the main menu of the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */
#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "core/Scene.h"
#include "ecs/GameObject.h"

/**
 * @class MenuScene
 * @brief Represents the main menu scene of the game.
 * @details The MenuScene class is responsible for managing the main menu of the game.
 *          It can be extended in the future to include methods for handling menu interactions,
 *          rendering menu items, and managing transitions to other scenes such as gameplay or settings.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *          The MenuScene can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete menu system.
 *          It can be used to organize menu items, handle user input, and manage rendering and updates for the main menu.
 *          The class can also be extended to include features such as animations, sound effects, and transitions between different menu states.
 */
class MenuScene final : public cbit::Scene {
public:
    void initialize() override;

    void update(float deltaTime) override;

    void render() override;

private:
    GameObject _button;
};


#endif //MENUSCENE_H
