#include "Display.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

Display::Display(uint64_t width, uint64_t height, const char *title) {
    
    lastTime = glfwGetTime();

    // Setup glfw window
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW" << std::endl; 
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    });
    

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
    


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
}

Display::~Display()
{
    
}

bool Display::isClosed()
{
    return isDisplayClosed;
}

void Display::Update(bool draw, bool pollevents)
{
    
}

void Display::Clear(GLfloat r, GLfloat g, GLfloat b)
{
    
}

bool Display::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Display::close()
{
    glfwSetWindowShouldClose(window, true);
}

void Display::drawImGuis()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    for(auto window : imGuiWindows) {
        
        ImGui::Begin(window.first.c_str());

        for(ImGuiWidget *widget : window.second) {
            if(ImGuiFloatSlider *floatSlider = dynamic_cast<ImGuiFloatSlider *>(widget)) {
                ImGui::SliderFloat(floatSlider->name.c_str(), floatSlider->value, floatSlider->min, floatSlider->max);
            } else if(ImGuiIntSlider *intSlider = dynamic_cast<ImGuiIntSlider *>(widget)) {
                ImGui::SliderInt(intSlider->name.c_str(), intSlider->value, intSlider->min, intSlider->max);
            } else if(ImGuiButton *button = dynamic_cast<ImGuiButton *>(widget)) {
                if(ImGui::Button(button->name.c_str())) {
                    button->callback();
                }
            }
        }
        ImGui::End();   
        
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Display::initImGui() {
    ImGui_ImplGlfw_InitForOpenGL(window, true);
}

void Display::shutdownImGui() {
    ImGui_ImplGlfw_Shutdown();
}

void Display::addIntSlider(std::string uiName, std::string valueName, int *value, int min, int max)
{
    ImGuiIntSlider *intSlider = new ImGuiIntSlider();
    intSlider->name = valueName;
    intSlider->value = value;
    intSlider->min = min;
    intSlider->max = max;
    imGuiWindows[uiName].push_back(intSlider);
}

void Display::addFloatSlider(std::string uiName, std::string valueName, float *value, float min, float max)
{
    ImGuiFloatSlider *floatSlider = new ImGuiFloatSlider();
    floatSlider->name = valueName;
    floatSlider->value = value;
    floatSlider->min = min;
    floatSlider->max = max;
    imGuiWindows[uiName].push_back(floatSlider);
}

void Display::addButton(std::string uiName, std::string buttonText, ImGuiButtonCallback callback)
{
    ImGuiButton *button = new ImGuiButton();
    button->name = buttonText;
    button->text = buttonText;
    button->callback = callback;
    imGuiWindows[uiName].push_back(button);
}

void Display::updateDeltaTime()
{
    double currentTime = glfwGetTime();

    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

float Display::getDeltaTime()
{
    return deltaTime;
}

GLFWwindow* Display::getWindow()
{
    return window;
}