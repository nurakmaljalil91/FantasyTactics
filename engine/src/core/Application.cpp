/**
 * @file    Application.cpp
 * @brief   Implementation of the Application class.
 * @details Application class to handle all the game logic and rendering.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "Application.h"
#include "utilities/Logger.h"

Application::Application(const int width, const int height, const std::string &title,
                         const bool fullscreen): _window(width, height, title, fullscreen),
                                                 _wireframe(false),
                                                 _fullscreen(fullscreen), _windowedWidth(width),
                                                 _windowedHeight(height) {
    Logger::log()->info("Welcome to Fantasy Tactics!");
    _initialize();
}

Application::~Application() = default;

void Application::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!_window.shouldClose()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        const float deltaTime = elapsed.count();
        lastTime = currentTime;

        _processInput(deltaTime);
        _scenesManager.update(deltaTime);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        int windowWidth, windowHeight;
        glfwGetFramebufferSize(_window.getHandle(), &windowWidth, &windowHeight);
        _scenesManager.render(windowWidth, windowHeight);

        _window.swapBuffers();
        _window.pollEvents();
    }
}

SceneManager *Application::getSceneManager() {
    return &_scenesManager;
}

void Application::_initialize() {
    // initialize scene manager
    Logger::log()->info("Initializing SceneManager");
}

void Application::_processInput(float deltaTime) {
    GLFWwindow *window = _window.getHandle();

    // Close the window if the Escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Toggle wireframe on F1 (edge-detect)
    static bool f1Handled = false;
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !f1Handled) {
        _wireframe = !_wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
        f1Handled = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
        f1Handled = false;
    }
    // Toggle fullscreen on F11
    static bool f11Handled = false;
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS && !f11Handled) {
        _fullscreen = !_fullscreen;
        if (_fullscreen) {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(window, nullptr, 100, 100, _windowedWidth, _windowedHeight, 0);
        }
        f11Handled = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_RELEASE) {
        f11Handled = false;
    }
}
