/**
 * @file    Window.h
 * @brief   Header file for the Window class.
 * @details The Window class represents a window in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */


#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <glad/glad.h>
#include "GLFW/glfw3.h"


/**
 * @class Window
 * @brief Represents a window in the application.
 * @details This class is currently a placeholder and does not implement any functionality.
 *          It can be extended in the future to include methods for creating, managing, and rendering
 *          a window in the application. The Window class can be used in conjunction with other graphics
 *          components such as rendering contexts, event handling, and user interface elements.
 *          It is designed to be used in a graphics rendering context where OpenGL or other graphics
 *          libraries are used for rendering 2D and 3D graphics.
 *          The class can be extended to include methods for handling window events, resizing,
 *          and managing the rendering loop. It can also be integrated with other components such as
 *          input handling, graphics rendering, and user interface management to create a complete
 *          application windowing system.
 *          The Window class can be used in various applications such as games, graphical user interfaces,
 *          and other applications that require a window for rendering graphics and handling user input.
 */
class Window {
public:
    /**
     * Default constructor.
     * Initializes an empty window object. The window will be created when the constructor with parameters is called.
     * @param width      Width of the window in pixels.
     * @param height     Height of the window in pixels.
     * @param title      Title of the window.
     * @param fullscreen If true, the window will be created in fullscreen mode; otherwise, it will be created in windowed mode.
     */
    Window(int width, int height, const std::string &title, bool fullscreen = false);

    /**
     * Default destructor.
     * Cleans up the window resources.
     */
    ~Window();

    /**
     * Checks if the window should close.
     * @return true if the window should close, false otherwise.
     */
    [[nodiscard]] bool shouldClose() const;

    /**
     * Polls events for the window.
     * @details This method processes events such as keyboard input, mouse movement, and window resizing
     */
    void pollEvents();

    /**
     * Swaps the front and back buffers of the window.
     * @details This method is used to display the rendered content on the screen.
     */
    void swapBuffers() const;

    /**
     * Gets the window.
     * @return window.
     */
    [[nodiscard]] GLFWwindow *getHandle() const;

private:
    GLFWwindow *_window;
    bool _isFullscreen;
};


#endif //WINDOW_H
