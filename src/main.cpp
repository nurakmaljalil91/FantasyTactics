#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>
#include <sstream>
#include "utilities/logger.h"
#include "graphics/Shader.h"
#include "graphics/Cube.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphics/cameras/IsometricCamera.h"
#include "graphics/Texture2D.h"
#include "graphics/cameras/OrbitCamera.h"

auto APP_TITLE = "Fantasy Tactics";
constexpr int windowWidth = 1200;
constexpr int windowHeight = 800;
GLFWwindow *glfwWindow = nullptr;
bool wireframe = false;
bool fullscreen = false;

void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);

void showFPS(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY);

bool initOpenGL();

int main() {
    //initialize logger
    Logger::initialize();

    Logger::log()->info("Welcome to Cbit Game Engines!");

    Logger::log()->info("Welcome to Fantasy Tactics!");

    if (auto success = initOpenGL(); !success) {
        Logger::log()->error("Failed to initialize OpenGL");
    }

    double lastTime = glfwGetTime();

    // Build and compile our shader program
    Shader shader;
    shader.loadShaders("resources/shaders/cel_shading_texture.vert", "resources/shaders/cel_shading_texture.frag");

    // Create a cube
    Cube cube;
    glm::vec3 cubePosition(0.0f, 0.0f, 0.0f);

    // Create a sphere
    // Sphere sphere;

    Texture2D texture;
    texture.loadTexture("resources/textures/crate.jpg", true);

    // Construct your isometric camera
    // Suppose you want an ortho box from -2..2 horizontally, -2..2 vertically
    // near=0.1, far=100.0
    // Instead of (-2,2), try something bigger:
    // IsometricCamera isometricCamera(-10.f, 10.f, -10.f, 10.f, -100.f, 100.f);


    // 3) Position and angles. For a typical isometric angle:
    // isometricCamera.setPosition(glm::vec3(0.f, 0.f, 5.f)); // "pull back" a bit
    // isometricCamera.setAngles(45.f, 35.264f);

    OrbitCamera orbitCamera({0.0f, 0.0f, 0.0f}, 10.0f);

    glfwSetWindowUserPointer(glfwWindow, &orbitCamera);
    glfwSetCursorPosCallback(glfwWindow, OrbitCamera::mouseCallback);
    glfwSetScrollCallback(glfwWindow,  OrbitCamera::scrollCallback);
    // optionally hide the cursor while dragging
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Render loop
    while (!glfwWindowShouldClose(glfwWindow)) {
        constexpr float moveSpeed = 3.0f;
        showFPS(glfwWindow);

        const double currentTime = glfwGetTime();
        auto deltaTime = static_cast<float>(currentTime - lastTime);

        lastTime = currentTime;

        // Handle input
        // if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) cubePosition.z -= moveSpeed * deltaTime;
        // if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) cubePosition.z += moveSpeed * deltaTime;
        // if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) cubePosition.x -= moveSpeed * deltaTime;
        // if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) cubePosition.x += moveSpeed * deltaTime;

        orbitCamera.updateCameraPosition();
        // Render commands here
        // ---------------------
        // specify the color of the background
        glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the Cel shader
        shader.use();

        // Bind the crate texture to texture unit 0
        texture.bind(0);

        // Tell the fragment shader that diffuseTexture is in GL_TEXTURE0
        const GLuint diffuseLocation = glGetUniformLocation(shader.getProgram(), "diffuseTexture");
        glUniform1i(static_cast<int>(diffuseLocation), 0);

        // Build model/view/projection
        auto model = glm::mat4(1.0f);

        model = glm::translate(model, cubePosition);
        glm::mat4 view = orbitCamera.getViewMatrix();
        glm::mat4 projection = orbitCamera.getProjectionMatrix(static_cast<float>(windowWidth/windowHeight));

        const GLuint modelLocation = glGetUniformLocation(shader.getProgram(), "uModel");
        const GLuint viewLocation = glGetUniformLocation(shader.getProgram(), "uView");
        const GLuint projectionLocation = glGetUniformLocation(shader.getProgram(), "uProjection");

        glUniformMatrix4fv(static_cast<int>(modelLocation), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(static_cast<int>(viewLocation), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(static_cast<int>(projectionLocation), 1, GL_FALSE, glm::value_ptr(projection));

        // Cel shading lighting uniforms
        glm::vec3 lightDir(1.0f, 1.0f, 1.0f);
        const GLuint lightDirectionLocation = glGetUniformLocation(shader.getProgram(), "lightDir");
        glUniform3fv(static_cast<int>(lightDirectionLocation), 1, glm::value_ptr(lightDir));

        glm::vec3 baseColor(0.8f, 0.8f, 0.1f);
        const GLuint baseColorLocation = glGetUniformLocation(shader.getProgram(), "baseColor");
        glUniform3fv(static_cast<int>(baseColorLocation), 1, glm::value_ptr(baseColor));

        // Draw the sphere
        cube.draw();

        // function to swap the front and back buffers
        glfwSwapBuffers(glfwWindow);
        // Check events are triggered (like input, etc)
        glfwPollEvents();

        lastTime = currentTime;
    }

    // optional: de-allocate all resources once they've outlived their purpose:

    glfwTerminate();
    return 0;
}

// Modify initOpenGL() to enable depth testing
bool initOpenGL() {
    // initialize GLFW
    if (!glfwInit()) {
        Logger::log()->error("Failed to initialize GLFW");
        return false;
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
            glfwWindow = glfwCreateWindow(vMode->width, vMode->height, APP_TITLE, monitor, nullptr);
            if (!glfwWindow) {
                Logger::log()->error("Failed to create GLFW window");
                glfwTerminate();
                return false;
            }
        }
    } else {
        glfwWindow = glfwCreateWindow(windowWidth, windowHeight, APP_TITLE, nullptr, nullptr);
    }

    // make the window's context current
    glfwMakeContextCurrent(glfwWindow);

    // initialize GLAD before calling any OpenGL functions
    gladLoadGL();

    // Set the required callback functions
    glfwSetKeyCallback(glfwWindow, glfw_onKey);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    glfwSetScrollCallback(glfwWindow, glfw_onMouseScroll);

    // OpenGL version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    Logger::log()->info("Renderer: {}", reinterpret_cast<const char *>(renderer));
    Logger::log()->info("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    Logger::log()->info("OpenGL Initialization Complete");


    // Hides and grabs cursor, unlimited movement
    //    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //    glfwSetCursorPos(glfwWindow, windowWidth / 2.0, windowHeight / 2.0);

    // specify the viewport of OpenGL in the window
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

// Is called whenever a key is pressed/released via GLFW
void glfw_onKey(GLFWwindow *window, const int key, int scancode, const int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);
            // Switch to fullscreen
            glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
        } else {
            // Switch back to windowed mode
            glfwSetWindowMonitor(window, nullptr, 100, 100, windowWidth, windowHeight, 0);
        }
    }
}

// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the pWindow caption bar.
void showFPS(GLFWwindow *window) {
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
        glfwSetWindowTitle(window, outs.str().c_str());
        frameCount = 0;
    }
    frameCount++;
}

// Is called when the pWindow is resized
void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);

    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    float scale  = 5.0f;
}

void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY) {
}

// Update stuff every frame
void update(double elapsedTime) {
}
