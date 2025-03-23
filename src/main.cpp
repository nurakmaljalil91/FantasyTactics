#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>
#include <sstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "graphics/FPSCamera.h"
#include "graphics/Mesh.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "utilities/logger.h"
#include "graphics/shader.h"
#include "graphics/Texture2D.h"
#include "graphics/Cube.h"

const char *APP_TITLE = "Fantasy Tactics";
constexpr int windowWidth = 1200;
constexpr int windowHeight = 800;
GLFWwindow *window = nullptr;
bool wireframe = false;
bool fullscreen = false;

FPSCamera fpsCamera(glm::vec3(0.0f, 2.0f, 10.0f));
constexpr double ZOOM_SENSITIVITY = -3.0;
constexpr float MOVE_SPEED = 5.0; // units per second
constexpr float MOUSE_SENSITIVITY = 0.1f;

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 0.5f, 0.2f);

// Shader and cube objects
Shader* cubeShader;
Cube* cube;
glm::mat4 projection;

void glfw_onKey(GLFWwindow *pWindow, int key, int scancode, int action, int mode);

void showFPS(GLFWwindow *pWindow);

void glfw_onFramebufferSize(GLFWwindow *pWindow, int width, int height);

void glfw_onMouseScroll(GLFWwindow *pWindow, double deltaX, double deltaY);

bool initOpenGL();

void update(double elapsedTime);

int main() {
    //initialize logger
    Logger::Get().Initialize();

    Logger::Log()->info("Welcome to Cbit Game Engines!");

    Logger::Log()->info("Welcome to Fantasy Tactics!");

    initOpenGL();

   
    double lastTime = glfwGetTime();

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        showFPS(window);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        glfwPollEvents();
        update(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the cube
        cubeShader->Use();
        cube->Draw();

        glfwSwapBuffers(window);
        lastTime = currentTime;
    }

    glfwTerminate();
    return 0;
}

// Modify initOpenGL() to enable depth testing
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
    glfwSetScrollCallback(window, glfw_onMouseScroll);

    // OpenGL version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    Logger::Log()->info("Renderer: {}", reinterpret_cast<const char *>(renderer));
    Logger::Log()->info("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    Logger::Log()->info("OpenGL Initialization Complete");


    // Hides and grabs cursor, unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);

    // specify the color of the background
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    // specify the viewport of OpenGL in the window
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);

    // Initialize shaders
    cubeShader = new Shader("resources/shaders/cube.vert", "resources/shaders/cube.frag");
    cube = new Cube();

    // Set up projection matrix
    projection = glm::perspective(glm::radians(fpsCamera.GetFOV()), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    return true;
}

// Is called whenever a key is pressed/released via GLFW
void glfw_onKey(GLFWwindow *pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        wireframe = !wireframe;
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        fullscreen = !fullscreen;
        if (fullscreen) {
            // Get primary monitor
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            // Switch to fullscreen
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            // Switch back to windowed mode
            glfwSetWindowMonitor(window, nullptr, 100, 100, windowWidth, windowHeight, 0);
        }
    }
}

// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the pWwindow caption bar.
void showFPS(GLFWwindow *pWindow) {
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
        glfwSetWindowTitle(pWindow, outs.str().c_str());
        frameCount = 0;
    }
    frameCount++;
}

// Is called when the pWindow is resized
void glfw_onFramebufferSize(GLFWwindow *pWindow, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_onMouseScroll(GLFWwindow *pWindow, double deltaX, double deltaY) {
    double fov = fpsCamera.GetFOV() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    fpsCamera.SetFOV((float) fov);
}

// Update stuff every frame
void update(double elapsedTime) {
    // Camera orientation
    double mouseX, mouseY;

    // Get the current mouse cursor position delta
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
    fpsCamera.Rotate((float) (windowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY,
                     (float) (windowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

    // Clamp mouse cursor to center of screen
    glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);

    // Camera FPS movement

    // Forward/backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        fpsCamera.Move(MOVE_SPEED * (float) elapsedTime * fpsCamera.GetLook());
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        fpsCamera.Move(MOVE_SPEED * (float) elapsedTime * -fpsCamera.GetLook());

    // Strafe left/right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        fpsCamera.Move(MOVE_SPEED * (float) elapsedTime * -fpsCamera.GetRight());
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        fpsCamera.Move(MOVE_SPEED * (float) elapsedTime * fpsCamera.GetRight());

    // Up/down
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        fpsCamera.Move(MOVE_SPEED * (float) elapsedTime * fpsCamera.GetUp());
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        fpsCamera.Move(MOVE_SPEED * (float) elapsedTime * -fpsCamera.GetUp());

    // Update cube transformations
    cubeShader->Use();
    glm::mat4 view = fpsCamera.GetViewMatrix();
    cubeShader->SetUniform("view", view);
    cubeShader->SetUniform("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cubeShader->SetUniform("model", model);
}

