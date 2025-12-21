/**
 * @file    Application.h
 * @brief   Header file for the Application class.
 * @details Application class to handle all the game logic and rendering.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "SceneManager.h"

/**
 * @class Application
 * @brief Represents the main application class.
 */
class Application {
public:
    /**
     * Constructor for the Application class.
     * @param windowConfig Configuration for the application window.
     */
    explicit Application(const WindowConfig &windowConfig);

    /**
     * Default destructor for the Application class.
     * Cleans up resources used by the application.
     */
    ~Application();

    /**
     * Runs the application.
     */
    void run();

    /**
     * Get SceneManager instance.
     * @return Reference to the SceneManager instance.
     */
    SceneManager *getSceneManager();

private:
    Window _window;
    SceneManager _scenesManager;

    bool _wireframe;
    bool _fullscreen;
    int _windowedWidth, _windowedHeight;


    /**
     * Initializes the application.
     * This method sets up the necessary parts for the application to run.
     */
    void _initialize();

    /**
     * Processes input from the user.
     */
    void _processInput(float deltaTime);
};


#endif //APPLICATION_H
