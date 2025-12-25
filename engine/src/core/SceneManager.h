/**
 * @file    SceneManager.h
 * @brief   Header file for the SceneManager class.
 * @details Manages scenes in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include "Scene.h"

namespace cbit {
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
         * Default constructor.
         * @details Initializes an empty SceneManager with no scenes.
         */
        SceneManager();

        /**
         * Default destructor.
         * @details Cleans up resources used by the SceneManager.
         */
        ~SceneManager();

        /**
         * Initialize the SceneManager with a GLFW window.
         * @param window specifies the GLFW window to be used by the SceneManager.
         */
        void initialize(GLFWwindow *window);

        /**
         * Add a scene to the manager.
         * @param name  Name of the scene to be added.
         * @param scene scene to be added.
         */
        void addScene(const std::string &name, std::shared_ptr<Scene> scene);

        /**
         * Get the current active scene.
         * @return A name of the current active scene.
         */
        [[nodiscard]] std::string getActiveScene();

        /**
         * Set the active scene by name.
         * @param name the name of the scene to be set as active.
         */
        void setActiveScene(const std::string &name);

        /**
         * Update all scenes in the manager.
         * @param deltaTime Time since the last update in seconds.
         */
        void update(float deltaTime) const;

        /**
         * Render all scenes in the manager.
         * @details This method calls the render method of each scene in the manager.
         * @param windowWidth  Width of the window in pixels.
         * @param windowHeight Height of the window in pixels.
         */
        void render(int windowWidth, int windowHeight) const;

        /**
         * Clean up resources used by the SceneManager.
         * @details This method cleans up resources used by the SceneManager, such as destroying scenes and freeing memory.
         */
        void cleanup() const;


    private:
        std::unordered_map<std::string, std::shared_ptr<Scene> > _scenes;
        std::shared_ptr<Scene> _currentScene;
        GLFWwindow *_window{nullptr};
    };
}
#endif //SCENEMANAGER_H
