/**
 * @file    Application.h
 * @brief   Header file for the Application class.
 * @details Application class to handle all the game logic and rendering.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#ifndef APPLICATION_H
#define APPLICATION_H
#include "SceneManager.h"
#include "Window.h"


/**
 * @class Application
 * @brief Represents the main application class.
 */
class Application {
public:
    /**
     * Constructor for the Application class.
     * @param width      Width of the application window.
     * @param height     Height of the application window.
     * @param title      Title of the application window.
     * @param fullscreen If true, the application will run in fullscreen mode; otherwise, it will run in windowed mode.
     */
    Application(int width, int height, const std::string &title, bool fullscreen = false);

    /**
     * Default destructor for the Application class.
     * Cleans up resources used by the application.
     */
    ~Application();

    /**
     * Runs the application.
     */
    void run();

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
