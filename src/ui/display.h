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
    std::string name_;
    
private:
    // effectively not used, just to be able to dynamically find out type of Widget
    virtual void PrintName() const {

    };
};

struct ImGuiIntSlider : public ImGuiWidget {
    int *value_;
    int min_;
    int max_;

private:
    void PrintName() const override {

    };
};

struct ImGuiFloatSlider : public ImGuiWidget {
    float *value_;
    float min_;
    float max_;

private:
    void PrintName() const override {

    }
};

struct ImGuiButton : public ImGuiWidget {
    std::string text_;
    ImGuiButtonCallback callback_;

    void PrintName() const override {

    }
};


class Display {
public:
    Display(uint64_t width, uint64_t height, const char *title);
    ~Display();
    bool IsClosed();
    void Update(bool draw, bool pollevents);
    void Clear(GLfloat r, GLfloat g, GLfloat b);
    bool ShouldClose();
    void Close();
    // void AddButton (Button b);
    // void CheckButtons(double x, double y);

    void DrawImGuis();
    void AddIntSlider(std::string ui_name, std::string value_name, int *value, int min, int max);
    void AddFloatSlider(std::string ui_name, std::string value_name, float *value, float min, float max);
    void AddButton(std::string ui_name, std::string button_text, ImGuiButtonCallback callback);

    void InitImGui();
    void ShutdownImGui();

    void UpdateDeltaTime();
    float GetDeltaTime();


    GLFWwindow *GetWindow();

    float delta_time_ = 0.0f;
    float last_time_ = 0.0f;

private:
    ImGuiIO io_;

    GLFWwindow *window_;
    // std::vector<Button> buttons;
    bool is_display_closed_;

    std::unordered_map<std::string, std::list<ImGuiWidget*>> im_gui_windows_;
};

#endif //DISPLAY_H