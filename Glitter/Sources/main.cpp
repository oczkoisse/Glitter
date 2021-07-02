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

const char *fragShader2Source = R"(
# version 330 core

out vec4 fragColor;

void main() {
    fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
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

int main(int argc, char *argv[]) {
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


    // 1. Bind vertex array object
    unsigned int vao[2];
    glGenVertexArrays(2, vao);

    unsigned int vbo[2];
    glGenBuffers(2, vbo);
    
    // Note that these values are already in NDC
    // First triangle
    float vertices1[] = {
        -0.5f, 0.5f, 0.0f,
        -1.0f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f
    };

    // Second triangle
    float vertices2[] = {
         0.0f, -0.5f, 0.0f,
         0.5f, 0.5f, 0.0f,
         1.0f, -0.5f, 0.0f
    };

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vao[1]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

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

    unsigned int fragShader2;
    fragShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader2, 1, &fragShader2Source, nullptr);
    glCompileShader(fragShader2);

    int fsuccess2;
    glGetShaderiv(fragShader2, GL_LINK_STATUS, &fsuccess2);
    if (!fsuccess2) {
        char infoLog[512];
        glGetShaderInfoLog(fragShader2, 512, nullptr, infoLog);
        std::cerr << "ERROR: Fragment shader 2 compilation failed\n" << infoLog << std::endl;
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

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(vertexShader, vertexShader);
    glAttachShader(shaderProgram2, fragShader2);
    glLinkProgram(shaderProgram2);

    int p2success;
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &p2success);
    if (!p2success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram2, 512, nullptr, infoLog);
        std::cerr << "ERROR: Program 2 linking failed\n" << infoLog << std::endl;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    glDeleteShader(fragShader2);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    // To enable wireframe mode
    // Front and back of all triangles should be drawn as lines
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {
        // process user input
        processInput(mWindow);

        // Rendering
        // Set the color to clear the color buffer to
        
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Flip buffers and draw (using double buffers)
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2, vbo);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram2);
    
    glfwTerminate();
    return EXIT_SUCCESS;
}

