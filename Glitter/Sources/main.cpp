// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>

const char *vShaderSource = R"(
#version 330 core

layout (location=0) in vec3 pos;

void main() {
    gl_Position = vec4(pos, 1.0);
}
)";

const char *fragShaderSource = R"(
# version 330 core

out vec4 fragColor;

void main() {
    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

void resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char * argv[]) {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window and associated OpenGL context
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        std::cerr << "Failed to create OpenGL context" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    std::cerr << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    glfwSetFramebufferSizeCallback(mWindow, resize_callback);

    // Note that these values are already in NDC
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom left triangle vertex
         0.5f, -0.5f, 0.0f, // Bottom right triangle vertex
         0.0f,  0.5f, 0.0f  // Top triangle vertex
    };

    // 1. Bind vertex array object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 2. Bind and set vertex buffers
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    // Bind the buffer to a target
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Send data to target
    // GL_STATIC_DRAW means the data is set only once and used many times
    // This hint allows a GPU to store the data in appropriate memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
    }

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
    glCompileShader(fragShader);

    int fsuccess;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fsuccess);
    if (!fsuccess) {
        char infoLog[512];
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    int psuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &psuccess);
    if (!psuccess) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR: Program linking failed\n" << infoLog << std::endl;
    }


    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {
        // process user input
        processInput(mWindow);

        // Rendering
        // Set the color to clear the color buffer to
        
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Flip buffers and draw (using double buffers)
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return EXIT_SUCCESS;
}

