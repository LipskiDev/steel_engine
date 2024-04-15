#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

// ImGui Imports
#include "imgui.h"
#include "../../dependencies/imgui/backends/imgui_impl_glfw.h"
#include "../../dependencies/imgui/backends/imgui_impl_opengl3.h"

#include <functional>

#include <unordered_map>
#include <list>
#include <string>

using ImGuiButtonCallback = std::function<void()>;

struct ImGuiWidget {
    std::string name;
    
private:
    // effectively not used, just to be able to dynamically find out type of Widget
    virtual void printName() const {

    };
};

struct ImGuiIntSlider : public ImGuiWidget {
    int *value;
    int min;
    int max;

private:
    void printName() const override {

    };
};

struct ImGuiFloatSlider : public ImGuiWidget {
    float *value;
    float min;
    float max;

private:
    void printName() const override {

    }
};

struct ImGuiButton : public ImGuiWidget {
    std::string text;
    ImGuiButtonCallback callback;

    void printName() const override {

    }
};


class Display {
public:
    Display(uint64_t width, uint64_t height, const char *title);
    ~Display();
    bool isClosed();
    void Update(bool draw, bool pollevents);
    void Clear(GLfloat r, GLfloat g, GLfloat b);
    bool shouldClose();
    void close();
    // void AddButton (Button b);
    // void CheckButtons(double x, double y);

    void drawImGuis();
    void addIntSlider(std::string uiName, std::string valueName, int *value, int min, int max);
    void addFloatSlider(std::string uiName, std::string valueName, float *value, float min, float max);
    void addButton(std::string uiName, std::string buttonText, ImGuiButtonCallback callback);

    void updateDeltaTime();
    float getDeltaTime();


    GLFWwindow *getWindow();

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

private:
    ImGuiIO io;

    GLFWwindow *window;
    // std::vector<Button> buttons;
    bool isDisplayClosed;

    std::unordered_map<std::string, std::list<ImGuiWidget*>> imGuiWindows;
};

#endif //DISPLAY_H