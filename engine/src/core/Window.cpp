/**
 * @file    Window.cpp
 * @brief   Implementation of the Window class.
 * @details The Window class represents a window in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "Window.h"
#include "utilities/Logger.h"

Window::Window(const int width, const int height, const std::string &title, const bool fullscreen): _window(nullptr),
    _isFullscreen(fullscreen) {
    Logger::initialize(); // Initialize the logger

    if (!glfwInit()) {
        Logger::log()->error("Failed to initialize GLFW");
    }

    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // required for Mac OS X (https://stackoverflow.com/questions/23706187/opengl-3-2-context-in-osx-with-glfw3
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE);


    // create a windowed mode window and its OpenGL context
    // Create an OpenGL 3.3 core, forward compatible context full-screen application
    if (_isFullscreen) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        if (const GLFWvidmode *vMode = glfwGetVideoMode(monitor)) {
            _window = glfwCreateWindow(vMode->width, vMode->height, title.c_str(), monitor, nullptr);
        }
    } else {
        _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }
    
    if (!_window) {
        Logger::log()->error("Failed to create GLFW window");
        glfwTerminate();
    } else {
        // make the window's context current
        glfwMakeContextCurrent(_window);
        // Initialize GLAD to load OpenGL functions
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        glfwSwapInterval(1); // Enable vsync
        Logger::log()->info("Window created successfully: {}x{}", width, height);
    }

    // specify the viewport of OpenGL in the window
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
}

Window::~Window() {
    if (_window) {
        glfwDestroyWindow(_window);
    }
    glfwTerminate();
    Logger::log()->info("Window destroyed and GLFW terminated");
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(_window);
}

GLFWwindow *Window::getHandle() const {
    return _window;
}
