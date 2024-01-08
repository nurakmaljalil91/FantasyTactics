#include <iostream>
#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>
#include <sstream>
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "utilities/logger.h"
#include "graphics/shader.h"
#include "graphics/Texture2D.h"

const char *APP_TITLE = "Fantasy Tactics";
constexpr int windowWidth = 800;
constexpr int windowHeight = 800;
GLFWwindow *window = nullptr;
bool wireframe = false;
const std::string texture1Filename = "assets/textures/crate.jpg";
const std::string texture2Filename = "assets/textures/airplane.png";

bool fullscreen = false;

void logTofile();

void glfw_onKey(GLFWwindow *pWindow, int key, int scancode, int action, int mode);

void showFPS(GLFWwindow *pWwindow);

void glfw_onFramebufferSize(GLFWwindow *pWindow, int width, int height);

bool initOpenGL();

int main() {
    //initialize logger
    Logger::Get().Initialize();

    Logger::Log()->info("Welcome to Cbit Game Engines!");


    Logger::Log()->info("Welcome to Fantasy Tactics!");

    initOpenGL();


    // Set up an array of vertices for a quad (2 triangls) with an index buffer data
    // (What is a vertex?)
    constexpr GLfloat vertices[] = {
        // position			 // tex coords
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top left
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // Top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f // Bottom left
    };

    const GLuint indices[] = {
        0, 1, 2, // First Triangle
        0, 2, 3 // Second Triangle
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    // Define a layout for the first vertex buffer "0"
    glEnableVertexAttribArray(0); // Enable the first attribute or attribute "0"

    // Texture Coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // set up element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // load shaders
    Shader shaderProgram;
    shaderProgram.LoadShaders("assets/shaders/default.vert", "assets/shaders/default.frag");

    // load textures
    Texture2D texture1;
    texture1.LoadTexture(texture1Filename, true);

    Texture2D texture2;
    texture2.LoadTexture(texture2Filename, true);

    // loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        showFPS(window);

        // poll for and process events
        glfwPollEvents();

        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);

        // render the quad
        // Must be called BEFORE setting uniforms because setting uniforms is done
        // on the currently active shader program.
        shaderProgram.Use();

        // set texture units
        texture1.Bind(0);
        texture2.Bind(1);

        // set uniforms
        glUniform1i(glGetUniformLocation(shaderProgram.GetProgram(), "texture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram.GetProgram(), "texture2"), 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // swap front and back buffers
        glfwSwapBuffers(window);
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // clean up glfw
    glfwTerminate();
    return 0;
}

// log to file
// TODO: make this logger class and move it to its own file
void logTofile() {
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic.txt", true));

    const auto combined_logger = std::make_shared<spdlog::logger>("FT logger", begin(sinks), end(sinks));
    spdlog::register_logger(combined_logger);

    combined_logger->info("Welcome to Fantasy Tactics logs!");
}

// initialize GLFW and OpenGL
bool initOpenGL() {
    // initialize GLFW
    if (!glfwInit()) {
        Logger::Log()->error("Failed to initialize GLFW");
        return true;
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
        if (const GLFWvidmode *vMode = glfwGetVideoMode(monitor)) {
            window = glfwCreateWindow(vMode->width, vMode->height, APP_TITLE, monitor, nullptr);
            if (!window) {
                Logger::Log()->error("Failed to create GLFW window");
                glfwTerminate();
                return true;
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
    Logger::Log()->info("Renderer: {}", reinterpret_cast<const char *>(renderer));
    Logger::Log()->info("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    Logger::Log()->info("OpenGL Initialization Complete");

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
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        wireframe = !wireframe;
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the pWwindow caption bar.
void showFPS(GLFWwindow *pWwindow) {
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    const double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float
    // limit text updates to 4 times per second
    if (const double elapsedSeconds = currentSeconds - previousSeconds; elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        const double fps = static_cast<double>(frameCount) / elapsedSeconds;
        const double msPerFrame = 1000.0 / fps;
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
