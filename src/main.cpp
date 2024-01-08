#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>
#include <sstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "graphics/FPSCamera.h"
#include "spdlog/spdlog.h"
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
const std::string crateImage = "resources/textures/wooden_crate.jpg";
const std::string gridImage = "resources/textures/grid.jpg";

bool fullscreen = false;

FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0, 1.0, 1.0));
constexpr double ZOOM_SENSITIVITY = -3.0;
constexpr float MOVE_SPEED = 5.0; // units per second
constexpr float MOUSE_SENSITIVITY = 0.1f;

void logTofile();

void glfw_onKey(GLFWwindow *pWindow, int key, int scancode, int action, int mode);

void showFPS(GLFWwindow *pWwindow);

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


    // Set up an array of vertices for a quad (2 triangls) with an index buffer data
    // (What is a vertex?)
    constexpr GLfloat cubeVertices[] = {
        // position		 // tex coords

        // front face
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

        // back face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        // left face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

        // right face
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        // bottom face
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
    };


    // Cube and floor positions
    glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 floorPos = glm::vec3(0.0f, -1.0f, 0.0f);


    // Set up buffers on the GPU
    GLuint VBO, VAO;
    // create vertex array object to store all the vertex attribute state
    // VAO are used to store which VBOs are associated with which attributes
    // VAO must be before VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    // Define a layout for the first vertex buffer "0"
    glEnableVertexAttribArray(0); // Enable the first attribute or attribute "0"

    // Texture Coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // load shaders
    Shader shaderProgram;
    shaderProgram.LoadShaders("resources/shaders/basic.vert", "resources/shaders/basic.frag");

    // load textures
    Texture2D texture;
    texture.LoadTexture(crateImage, true);

    Texture2D floorTexture;
    floorTexture.LoadTexture(gridImage, true);

    double lastTime = glfwGetTime();

    // loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        showFPS(window);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        // poll for and process events
        glfwPollEvents();

        update(deltaTime);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set texture units
        texture.Bind(0);
        glm::mat4 model(1.0), view(1.0), projection(1.0);

        // Update the cube position (don't really need to do this every frame because it isn't changing)
        model = glm::translate(model, cubePos);

        // Create the View matrix
        view = fpsCamera.GetViewMatrix();

        // Create the projection matrix
        projection = glm::perspective(glm::radians(fpsCamera.GetFOV()), (float) windowWidth / (float) windowHeight,
                                      0.1f, 100.0f);

        // Render the scene
        // Must be called BEFORE setting uniforms because setting uniforms is done
        // on the currently active shader program.
        shaderProgram.Use();

        // Pass the matrices to the shader
        shaderProgram.SetUniform("model", model);
        shaderProgram.SetUniform("view", view);
        shaderProgram.SetUniform("projection", projection);

        glBindVertexArray(VAO);

        // Draw the crate
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Position and render the floor (a squashed and scaled cube!)
        // Make the floor texture "active" in the shaders
        floorTexture.Bind(0);

        model = glm::translate(model, floorPos) * glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));

        // Send the model matrix for the floor to the vertex shader
        shaderProgram.SetUniform("model", model);

        // Draw the floor
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        // swap front and back buffers
        glfwSwapBuffers(window);

        lastTime = currentTime;
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
}
