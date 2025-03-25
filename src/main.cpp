#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>
#include <sstream>
#include "utilities/logger.h"
#include "graphics/Shader.h"

const char *APP_TITLE = "Fantasy Tactics";
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

    auto success = initOpenGL();

    if (!success) {
        Logger::log()->error("Failed to initialize OpenGL");
    }

    double lastTime = glfwGetTime();

    // Build and compile our shader program
    Shader shader;
    shader.loadShaders("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(glfwWindow)) {
        showFPS(glfwWindow);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        // Render commands here
        // ---------------------
        // specify the color of the background
        glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        shader.use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // function to swap the front and back buffers
        glfwSwapBuffers(glfwWindow);
        // Check events are triggered (like input, etc)
        glfwPollEvents();

        lastTime = currentTime;
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode) {
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
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
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
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY) {

}

// Update stuff every frame
void update(double elapsedTime) {
  
}

