#include "display.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

Display::Display(uint64_t width, uint64_t height, const char *title) {
    
    last_time_ = glfwGetTime();

    // Setup glfw window
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW" << std::endl; 
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window_ == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
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
    io_ = ImGui::GetIO(); (void)io_;
    io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io_.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

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

bool Display::IsClosed()
{
    return is_display_closed_;
}

void Display::Update(bool draw, bool pollevents)
{
    
}

void Display::Clear(GLfloat r, GLfloat g, GLfloat b)
{
    
}

bool Display::ShouldClose()
{
    return glfwWindowShouldClose(window_);
}

void Display::Close()
{
    glfwSetWindowShouldClose(window_, true);
}

void Display::DrawImGuis()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    for(auto win : im_gui_windows_) {
        
        ImGui::Begin(win.first.c_str());

        for(ImGuiWidget *widget : win.second) {
            if(ImGuiFloatSlider *float_slider = dynamic_cast<ImGuiFloatSlider *>(widget)) {
                ImGui::SliderFloat(float_slider->name_.c_str(), float_slider->value_, float_slider->min_, float_slider->max_);
            } else if(ImGuiIntSlider *int_slider = dynamic_cast<ImGuiIntSlider *>(widget)) {
                ImGui::SliderInt(int_slider->name_.c_str(), int_slider->value_, int_slider->min_, int_slider->max_);
            } else if(ImGuiButton *button = dynamic_cast<ImGuiButton *>(widget)) {
                if(ImGui::Button(button->name_.c_str())) {
                    button->callback_();
                }
            }
        }
        ImGui::End();   
        
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Display::InitImGui() {
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
}

void Display::ShutdownImGui() {
    ImGui_ImplGlfw_Shutdown();
}

void Display::AddIntSlider(std::string ui_name, std::string value_name, int *value, int min, int max)
{
    ImGuiIntSlider *int_slider = new ImGuiIntSlider();
    int_slider->name_ = value_name;
    int_slider->value_ = value;
    int_slider->min_ = min;
    int_slider->max_ = max;
    im_gui_windows_[ui_name].push_back(int_slider);
}

void Display::AddFloatSlider(std::string ui_name, std::string value_name, float *value, float min, float max)
{
    ImGuiFloatSlider *float_slider = new ImGuiFloatSlider();
    float_slider->name_ = value_name;
    float_slider->value_ = value;
    float_slider->min_ = min;
    float_slider->max_ = max;
    im_gui_windows_[ui_name].push_back(float_slider);
}

void Display::AddButton(std::string ui_name, std::string button_text, ImGuiButtonCallback callback)
{
    ImGuiButton *button = new ImGuiButton();
    button->name_ = button_text;
    button->text_ = button_text;
    button->callback_ = callback;
    im_gui_windows_[ui_name].push_back(button);
}

void Display::UpdateDeltaTime()
{
    double current_time = glfwGetTime();

    delta_time_ = current_time - last_time_;
    last_time_ = current_time;
}

float Display::GetDeltaTime()
{
    return delta_time_;
}

GLFWwindow* Display::GetWindow()
{
    return window_;
}