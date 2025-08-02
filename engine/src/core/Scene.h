/**
 * @file    Scene.h
 * @brief   Scene class for managing game scenes.
 * @details The Scene class is a placeholder for managing game scenes in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#ifndef SCENE_H
#define SCENE_H

#include "OpenGLInclude.h"
#include "ecs/EntityComponentSystem.h"

/**
 * @class Scene
 * @brief Scene class for managing game scenes.
 * @details The Scene class is a placeholder for managing game scenes in the application.
 *          It can be extended in the future to include methods for loading, updating, and rendering
 *          game scenes, handling scene transitions, and managing game objects within a scene.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *          The Scene class can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete game scene management system.
 *          It can be used to organize game objects, handle scene-specific logic, and manage rendering
 *          and updates for the game scene.
 */
class Scene {
public:
    /**
     * Default deconstructor.
     * @details Cleans up resources associated with the scene.
     */
    virtual ~Scene() = default;

    /**
     * Initializes the scene.
     * @details This method is called to set up the scene, load resources, and prepare for rendering.
     */
    virtual void initialize() = 0;

    /**
     * Updates the scene.
     * @param deltaTime Time since the last update in seconds.
     * @details This method is called to update the scene logic, handle input, and perform any necessary updates.
     */
    virtual void update(float deltaTime);

    /**
     * Renders the scene.
     * @details This method is called to render the scene, drawing all game objects and UI elements.
     */
    virtual void render();

    /**
     * Cleans up the scene.
     * @details This method is called to clean up resources used by the scene, such as textures, shaders, and game objects.
     *          It should be called when the scene is no longer needed, such as when switching scenes or exiting the application.
     */
    void cleanup();

    /**
   * Sets the GLFW window associated with the scene.
   * @param window Pointer to the GLFW window.
   * @details This method is used to associate a GLFW window with the scene, allowing the scene to access window properties and events.
   */
    void setWindow(GLFWwindow *window);

    /**
     * Sets the window size for the scene.
     * @param width  Width of the window in pixels.
     * @param height of the window in pixels.
     * @details This method is used to set the dimensions of the window in which the scene will be rendered.
     */
    void setWindowSize(int width, int height);

protected:
    /**
     * Gets the GLFW window associated with the scene.
     * @return Pointer to the GLFW window.
     */
    [[nodiscard]] GLFWwindow *getWindow() const;

    /**
     * Gets the width of the window.
     * @return Width of the window in pixels.
     */
    [[nodiscard]] int getWindowWidth() const;

    /**
     * Gets the height of the window.
     * @return Height of the window in pixels.
     */
    [[nodiscard]] int getWindowHeight() const;

    /**
     * Gets the EntityComponentSystem associated with the scene.
     * @return Reference to the EntityComponentSystem instance.
     * @details This method provides access to the ECS, allowing the scene to manage game objects and their components.
     */
    [[nodiscard]] EntityComponentSystem &getWorld();

private:
    int _windowWidth = 0, _windowHeight = 0;
    GLFWwindow *_window = nullptr; // Pointer to the GLFW window associated with the scene
    EntityComponentSystem _world;
};


#endif //SCENE_H
