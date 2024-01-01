#include <iostream>
#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>
#include <sstream>
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"

#include "shaderProgram.h"

const char *APP_TITLE = "Fantasy Tactics";
const int windowWidth = 800;
const int windowHeight = 800;
GLFWwindow *window = nullptr;
bool wireframe = false;

bool fullscreen = false;

void glfw_onKey(GLFWwindow *pWindow, int key, int scancode, int action, int mode);

void showFPS(GLFWwindow *pWwindow);

void glfw_onFramebufferSize(GLFWwindow *pWindow, int width, int height);

bool initOpenGL();

int main() {

    spdlog::info("Welcome to Fantasy Tactics!");

    initOpenGL();

    // set up the triangles vertex data and attribute pointers
    GLfloat vertices[] = {
            -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // left
            0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // right
            0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  // top
            -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // left
            0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // right
            0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f   // bottom
    };

    // indices for the triangles
    GLuint indices[] = {
            0, 3, 5, // lower left triangle
            3, 2, 4, // lower right triangle
            5, 4, 1  // upper triangle
    };


    // Set up buffers on the GPU
    GLuint VBO, VAO, EBO;
    // create vertex array object to store all the vertex attribute state
    // VAO are used to store which VBOs are associated with which attributes
    // VAO must be before VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
    // enable the vertex attribute
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("assets/shaders/default.vert", "assets/shaders/default.frag");


    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        showFPS(window);
        // render here
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // clean up glfw
    glfwTerminate();
    return 0;
}

// initialize GLFW and OpenGL
bool initOpenGL() {
    // initialize GLFW
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
        return -1;
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
    // Create an OpenGL 3.3 core, forward compatible context full screen application
    if (fullscreen) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *vMode = glfwGetVideoMode(monitor);
        if (vMode) {
            window = glfwCreateWindow(vMode->width, vMode->height, APP_TITLE, monitor, nullptr);
            if (!window) {
                spdlog::error("Failed to create GLFW window");
                glfwTerminate();
                return -1;
            }
        }
    } else {
        window = glfwCreateWindow(windowWidth, windowHeight, APP_TITLE, nullptr, nullptr);
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

    // initialize GLAD before calling any OpenGL functions
    gladLoadGL();

    // Set the required callback functions
    glfwSetKeyCallback(window, glfw_onKey);
    glfwSetFramebufferSizeCallback(window, glfw_onFramebufferSize);

    // OpenGL version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    spdlog::info("Renderer: {}", reinterpret_cast<const char *>(renderer));
    spdlog::info("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    spdlog::info("OpenGL Initialization Complete");

    // specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    // specify the viewport of OpenGL in the window
    glViewport(0, 0, windowWidth, windowHeight);

    return true;

}

// Is called whenever a key is pressed/released via GLFW
void glfw_onKey(GLFWwindow *pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
}

// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the pWwindow caption bar.
void showFPS(GLFWwindow *pWwindow) {
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float
    elapsedSeconds = currentSeconds - previousSeconds;
    // limit text updates to 4 times per second
    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double) frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;
        std::ostringstream outs;
        outs.precision(3); // decimal places
        outs << std::fixed
             << APP_TITLE << "    "
             << "FPS: " << fps << "    "
             << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(pWwindow, outs.str().c_str());
        frameCount = 0;
    }
    frameCount++;
}

// Is called when the pWindow is resized
void glfw_onFramebufferSize(GLFWwindow *pWindow, int width, int height) {
    glViewport(0, 0, width, height);
}