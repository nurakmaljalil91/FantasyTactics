#include <iostream>
#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>
#include <sstream>
#include "spdlog/spdlog.h"

const char *APP_TITLE = "Fantasy Tactics";
const int windowWidth = 800;
const int windowHeight = 800;
bool fullscreen = false;

const char *vertexShaderSrc = "#version 330 core\n"
                              "layout (location = 0) in vec3 aPos;\n"
                              "void main()\n"
                              "{\n"
                              "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                              "}\0";

const char *fragmentShaderSrc = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "void main()\n"
                                "{\n"
                                "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                "}\n\0";

void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void showFPS(GLFWwindow *window) {
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
        glfwSetWindowTitle(window, outs.str().c_str());
        frameCount = 0;
    }
    frameCount++;
}

void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main() {

    spdlog::info("Welcome to Fantasy Tactics!");

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

    GLfloat vertices[] = {
            -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // left
            0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // right
            0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  // top
            -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // left
            0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // right
            0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f   // bottom
    };

    GLuint indices[] = {
            0, 3, 5, // lower left triangle
            3, 2, 4, // lower right triangle
            5, 4, 1  // upper triangle
    };

    // create a windowed mode window and its OpenGL context
    // Create an OpenGL 3.3 core, forward compatible context full screen application
    GLFWwindow* window = nullptr;
    
    if (fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vMode = glfwGetVideoMode(monitor);
        if (vMode)
        {
            window = glfwCreateWindow(vMode->width, vMode->height, APP_TITLE, monitor, NULL);
            if (!window) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
            }
        }
    }
    else {
        window = glfwCreateWindow(windowWidth, windowHeight, APP_TITLE, nullptr, nullptr);
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, glfw_onKey);
    glfwSetFramebufferSizeCallback(window, glfw_onFramebufferSize);


    // initialize GLAD before calling any OpenGL functions
    gladLoadGL();

    // OpenGL version info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    spdlog::info("Renderer: {}", reinterpret_cast<const char *>(renderer));
    spdlog::info("OpenGL version supported: {}", reinterpret_cast<const char *>(version));
    spdlog::info("OpenGL Initialization Complete");

    // specify the viewport of OpenGL in the window
    glViewport(0, 0, windowWidth, windowHeight);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);

    // create a shader program object
    GLuint shaderProgram = glCreateProgram();

    // attach the vertex and fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // link all the shaders together into a single shader program
    glLinkProgram(shaderProgram);

    // delete the vertex and fragment shaders once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO,VBO, EBO;
    // create vertex array object to store all the vertex attribute state
    // VAO are used to store which VBOs are associated with which attributes
    // VAO must be before VBO
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);

    // enable the vertex attribute
    glEnableVertexAttribArray(0);

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        showFPS(window);
        // render here
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
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
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shaderProgram);

    // clean up glfw
    glfwTerminate();
    return 0;
}
