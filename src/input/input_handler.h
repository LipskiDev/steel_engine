#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../ui/Display.h"
#include <functional>
#include <unordered_map>

using KeyCallbackFunction = std::function<void()>;
using MouseCallbackFunction = std::function<void(double, double)>;

class InputHandler {
private:
    InputHandler();
    static InputHandler *input_handler_;

    std::unordered_map<int, std::list<KeyCallbackFunction>> key_hold_callbacks_;
    std::unordered_map<int, std::list<KeyCallbackFunction>> key_press_callbacks_;
    std::unordered_map<int, std::list<KeyCallbackFunction>> key_release_callbacks_;

    std::unordered_map<int, std::list<KeyCallbackFunction>> button_hold_callbacks_;
    std::unordered_map<int, std::list<KeyCallbackFunction>> button_press_callbacks_;
    std::unordered_map<int, std::list<KeyCallbackFunction>> button_release_callbacks_;

    std::list<MouseCallbackFunction> mouse_callbacks_;

    std::list<int> currently_pressed_keys_{};
    std::list<int> currently_pressed_buttons_{};

    // Mouse Input
    
    bool first_mouse_ = true;
    float last_x_ = 0;
    float last_y_ = 0;

    bool is_cursor_hidden_ = false;


public:

    

    InputHandler(InputHandler &other) = delete;

    void operator=(const InputHandler &) = delete;

    static InputHandler *Instance();

    void AddKeyHoldCallback(int key, KeyCallbackFunction callback);
         
    void AddKeyPressCallback(int key, KeyCallbackFunction callback);
         
    void AddKeyReleaseCallback(int key, KeyCallbackFunction callback);
         
    void AddButtonHoldCallback(int key, KeyCallbackFunction callback);
         
    void AddButtonPressCallback(int key, KeyCallbackFunction callback);
         
    void AddButtonReleaseCallback(int key, KeyCallbackFunction callback);
         
    void AddMouseMoveCallback(MouseCallbackFunction callback);

    void CalcMouseOffset(double x, double y, double &x_offset, double &y_offset);

    static void RegisterKeys(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void RegisterButtons(GLFWwindow *window, int button, int action, int mods);

    static void ProcessMouseInput(GLFWwindow *window, double x_pos, double y_pos);

    void HideCursor(GLFWwindow *window);

    void ShowCursor(GLFWwindow *window);

    void ProcessInput();

    void AddKeyPress(int key);

    void RemoveKeyPress(int key);

    void AddButtonPress(int button);

    void RemoveButtonPress(int button);

    void UpdateMousePos(GLFWwindow *window, double x_pos, double y_pos);

    bool IsCursorHidden();

};

#endif // INPUT_HANDLER_H