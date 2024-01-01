#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "utils/Shader.h"
#include "terrain/terrain.h"
#include "Camera.h"
#include "mesh/Mesh.h"

#include "config.h"

GLFWwindow *window;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

Camera mainCamera(glm::vec3(0.0f, 2.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.0;
float lastY = WINDOW_HEIGHT / 2.0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
int loadTexture(char const *path);
void setup();

int main()
{
    setup();

    Shader mainShader(ROOT_DIR"/assets/terrainVertex.glsl", ROOT_DIR"/assets/terrainFragment.glsl"); 

    int modelLocation = mainShader.getUniformLocation("model");
    int viewLocation = mainShader.getUniformLocation("view");
    int projectionLocation = mainShader.getUniformLocation("projection");
    int viewPosLocation = mainShader.getUniformLocation("viewPos");
    int dirLightDirectionLocation = mainShader.getUniformLocation("dirLight.direction");
    int dirLightAmbientLocation = mainShader.getUniformLocation("dirLight.ambient");
    int dirLightDiffuseLocation = mainShader.getUniformLocation("dirLight.diffuse");
    int dirLightSpecularLocation = mainShader.getUniformLocation("dirLight.specular");

    //Terrain Stuff
    BaseTerrain bt(8, 2.f);
    bt.initTerrain();
    bt.setShader(mainShader);


    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.f);

    mainShader.bind();
    Shader::setVec3(dirLightDirectionLocation, glm::vec3(1.0, 0.5, 0.2));
    Shader::setVec3(dirLightAmbientLocation, glm::vec3(1.0, 0.5, 0.2));
    Shader::setVec3(dirLightDiffuseLocation, glm::vec3(1.0, 0.5, 0.2));
    Shader::setVec3(dirLightSpecularLocation, glm::vec3(1.0, 0.5, 0.2));


    while (!glfwWindowShouldClose(window))
    {

        // Calc deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Draw Background Color
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = mainCamera.getViewMat();
        Shader::setMat4(projectionLocation, projection);
        Shader::setMat4(viewLocation, mainCamera.getViewMat());
        Shader::setMat4(modelLocation, glm::mat4(1.0));
        Shader::setVec3(viewPosLocation, mainCamera.Position);

        // Shader::setVec3()

        bt.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// Adjusts OpenGL's Viewport size based on window size
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    mainCamera.ProcessMouseInput(xoffset, yoffset, false);
}

// Processes User Input
void processInput(GLFWwindow *window)
{
    float cameraSpeed = 10.f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == true)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera.ProcessKeyboard(FORWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera.ProcessKeyboard(BACKWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera.ProcessKeyboard(LEFT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera.ProcessKeyboard(RIGHT, cameraSpeed);
}


void setup() {
    // Setup glfw window
    
    

    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW" << std::endl; 
        exit(-1);
    }
    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Steel Engine", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (gl3wInit()) {
        std::cerr << "Failed to init gl3w" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
}