#include <iostream>

#include "../lib/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

#include "Shader.h"

// Adjusts OpenGL's Viewport size based on window size
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Processes User Input
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == true) {
        glfwSetWindowShouldClose(window, true);
    }
}


int main() {


    // Setup glfw window
    GLFWwindow *window;
    
    if(!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Steel Engine", NULL, NULL);

    if(window == NULL) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Load OpenGL
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress )) {
        std::cout << "Couldn't load OpenGL\n";
        glfwTerminate();
        return -1;
    }

    // Setup Shader
    Shader shader("../shaders/vertexShader.glsl", "../shaders/fragmentShader.glsl");

    // Triangle Vert Positions and Colors
    float vertices[] = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top   
    }; 

    GLuint VBO, VAO;

    // Create Buffer and Array Objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind Buffer and Array Objects for subsequent use
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);



    while(!glfwWindowShouldClose(window)) {

        processInput(window);
        
        // Draw Background Color
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Apply Shaders
        shader.use();

        // Draw Triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}