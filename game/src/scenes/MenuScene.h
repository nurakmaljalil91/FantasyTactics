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
#include "graphics/renderers/ShaderProgram.h"
#include "graphics/renderers/TextRenderer.h"
#include "graphics/renderers/Texture2D.h"
#include "graphics/ui/Quad2D.h"

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
class MenuScene final : public Scene {
public:
    void initialize() override;

    void update(float deltaTime) override;

    void render() override;

private:
    ShaderProgram _uiShader;
    TextRenderer _textRenderer{1200, 800};
    Quad2D _buttonQuad;
    Texture2D _buttonTex;
};


#endif //MENUSCENE_H
