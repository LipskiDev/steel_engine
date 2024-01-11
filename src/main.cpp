#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <sstream>

// ImGui Imports
#include "imgui.h"
#include "../dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../dependencies/imgui/backends/imgui_impl_opengl3.h"

// GLM Imports
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "utils/Shader.h"
#include "terrain/terrain.h"
#include "terrain/PerlinNoiseChunkGenerator.h"
#include "objects/Camera.h"
#include "mesh/Mesh.h"
#include "objects/DirectionalLight.h"

#include "config.h"

GLFWwindow *window;

ImGuiIO io;

#define WINDOW_WIDTH 1920   
#define WINDOW_HEIGHT 1080

Camera *mainCamera;

float deltaTime = 0.0f;
float lastTime = 0.0f;
float lastFrame = 0.0f;
int nr_frames = 0;

bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.0;
float lastY = WINDOW_HEIGHT / 2.0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
int loadTexture(char const *path);
void setup();
void updateFPS();

int main()
{
    setup();

    Shader mainShader(ROOT_DIR"/assets/shaders/terrainVertex.glsl", ROOT_DIR"/assets/shaders/terrainFragment.glsl"); 

    //Terrain Stuff 
    DiamondSquareTerrain *bt = new DiamondSquareTerrain(10, 2.0f, mainShader);

    PerlinNoiseChunkGenerator generator;

    uint32_t waterHeightLocation = mainShader.getUniformLocation("waterHeight");
    uint32_t meshHeightLocation = mainShader.getUniformLocation("meshHeight");

    Shader::setFloat(waterHeightLocation, generator.getWaterHeight());
    Shader::setFloat(meshHeightLocation, generator.getMeshHeight());
    

    DirectionalLight dl(
        glm::vec3(1.0, -0.5, 0.2),  // Direction
        glm::vec3(0.2, 0.2, 0.2),   // Ambient
        glm::vec3(0.9, 0.9, 0.9),   // Diffuse
        glm::vec3(0.5, 0.5, 0.5)    // Specular
    );

    mainCamera = new Camera(
        glm::vec3(0.0f, 2.0f, 3.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.f,
        0.0f,
        45.f,
        (float) WINDOW_WIDTH / WINDOW_HEIGHT,
        0.1f,
        10000.f
    );
    mainShader.bind();
    
    mainShader.addDirectionalLight(dl);
    mainShader.addCamera(mainCamera);

    float scale = 1.0f;

    int scaleLocation = mainShader.getUniformLocation("scale");
    int modelLocation = mainShader.getUniformLocation("model");
    Shader::setFloat(scaleLocation, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window))
    {
        // Calc deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        updateFPS();

        // Draw Background Color
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        {
            ImGui::Begin("Terrain");

            // if(ImGui::SliderFloat("Scale", &scale, 0.001f, 5.0f)) {
            //     // bt->updateHeight(scale);
            // }

            ImGui::SliderFloat("Camera Speed", &mainCamera->Speed, 1.f, 10.0f);

            ImGui::End();
        }

        Shader::setFloat(scaleLocation, scale);
        bt->updateHeight(scale);
        
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        

        processInput(window);
        mainShader.updateCamera();

        mainShader.bind();

        ImGui::Render();
        //bt->Render();
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, (glm::vec3(-generator.getChunkWidth() / 2.0 + (generator.getChunkWidth() - 1) * i, 0.0f,  -generator.getChunkHeight() / 2.0 + (generator.getChunkHeight() - 1) * j) / 2.0f));

                Shader::setMat4(modelLocation, model);
                generator.renderChunk(i, j);
            }
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

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
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

        mainCamera->ProcessMouseInput(xoffset, yoffset, true);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
    
}

// Processes User Input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera->ProcessKeyboard(RIGHT, deltaTime);
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

    glfwSwapInterval(0);

    if (gl3wInit()) {
        std::cerr << "Failed to init gl3w" << std::endl;
        exit(-1);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
}

void updateFPS()
{
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    ++nr_frames;
    if(delta >= 1.0) {
        double fps = double(nr_frames) / delta;
        std::stringstream ss;
        ss << "Steel Engine - " << fps << " FPS";
        glfwSetWindowTitle(window, ss.str().c_str());
        nr_frames = 0;
        lastTime = currentTime;
    }
}
