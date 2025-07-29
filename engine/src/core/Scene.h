/**
 * @file    Scene.h
 * @brief   Scene class for managing game scenes.
 * @details The Scene class is a placeholder for managing game scenes in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#ifndef SCENE_H
#define SCENE_H

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
    virtual void update(float deltaTime) = 0;

    /**
     * Renders the scene.
     * @details This method is called to render the scene, drawing all game objects and UI elements.
     */
    virtual void render() const = 0;
};


#endif //SCENE_H
