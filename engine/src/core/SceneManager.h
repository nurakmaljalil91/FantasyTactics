/**
 * @file    SceneManager.h
 * @brief   Header file for the SceneManager class.
 * @details Manages scenes in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <vector>

#include "Scene.h"

/**
 * @class SceneManager
 * @brief Manages multiple scenes in the application.
 * @details The SceneManager class is responsible for managing multiple scenes in the application.
 *          It allows adding scenes, updating them, and rendering them in the correct order.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *          The SceneManager can be used to organize game scenes
 */
class SceneManager {
public:
    /**
     * Add a scene to the manager.
     * @param scene scene to be added.
     */
    void addScene(const std::shared_ptr<Scene> &scene);

    /**
     * Update all scenes in the manager.
     * @param deltaTime Time since the last update in seconds.
     */
    void update(float deltaTime) const;

    /**
     * Render all scenes in the manager.
     * @details This method calls the render method of each scene in the manager.
     */
    void render() const;

private:
    std::vector<std::shared_ptr<Scene> > _scenes;
};


#endif //SCENEMANAGER_H
