#include "input_handler.h"

#include <GLFW/glfw3.h>
#include "iostream"


InputHandler *InputHandler::input_handler_ = nullptr;

InputHandler::InputHandler() {

}

InputHandler *InputHandler::Instance()
{
    if(!input_handler_) {
        input_handler_ = new InputHandler();
    }
    return input_handler_;
}

void InputHandler::AddKeyHoldCallback(int key, KeyCallbackFunction callback)
{
    key_hold_callbacks_[key].push_back(callback);
}

void InputHandler::AddKeyPressCallback(int key, KeyCallbackFunction callback)
{
    key_press_callbacks_[key].push_back(callback);
}

void InputHandler::AddKeyReleaseCallback(int key, KeyCallbackFunction callback)
{
    key_release_callbacks_[key].push_back(callback);
}

void InputHandler::AddMouseMoveCallback(MouseCallbackFunction callback)
{
    mouse_callbacks_.push_back(callback);
}

void InputHandler::RegisterKeys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        auto it = input_handler_->key_press_callbacks_.find(key);
        if(it != input_handler_->key_press_callbacks_.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }
        InputHandler::Instance()->AddKeyPress(key);
    } else if(action == GLFW_RELEASE) {
        auto it = input_handler_->key_release_callbacks_.find(key);
        if(it != input_handler_->key_release_callbacks_.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }
        InputHandler::Instance()->RemoveKeyPress(key);
    }
}

void InputHandler::RegisterButtons(GLFWwindow *window, int button, int action, int mods) {
    if(action == GLFW_PRESS) {
        auto it = input_handler_->button_press_callbacks_.find(button);
        if(it != input_handler_->button_press_callbacks_.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }

        InputHandler::Instance()->AddButtonPress(button);
    } else if(action == GLFW_RELEASE) {
        auto it = input_handler_->button_release_callbacks_.find(button);
        if(it != input_handler_->button_release_callbacks_.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }

        InputHandler::Instance()->RemoveButtonPress(button);
    }
}

void InputHandler::ProcessMouseInput(GLFWwindow *window, double x_pos, double y_pos)
{
    InputHandler *input_handler_ = InputHandler::Instance();

    if(!input_handler_->is_cursor_hidden_) {
        return;
    }

    double x_offset, y_offset;

    input_handler_->CalcMouseOffset(x_pos, y_pos, x_offset, y_offset);


    for(auto callback : InputHandler::Instance()->mouse_callbacks_) {
        callback(x_offset, y_offset);
    }
}

void InputHandler::CalcMouseOffset(double x, double y, double &x_offset, double &y_offset)
{
    if(first_mouse_) {
        last_x_ = x;
        last_y_ = y;
        first_mouse_ = false;
    }

    x_offset = x - last_x_;
    y_offset = y - last_y_;
    last_x_ = x;
    last_y_ = y;
}

void InputHandler::ProcessInput() {
    for(auto key : currently_pressed_keys_) {
        auto it = input_handler_->key_hold_callbacks_.find(key);
        if(it != input_handler_->key_hold_callbacks_.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }
    }   

    for(auto button : currently_pressed_buttons_) {
        auto it = input_handler_->button_hold_callbacks_.find(button);
        if(it != input_handler_->button_hold_callbacks_.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }
    }
}

void InputHandler::HideCursor(GLFWwindow *window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    is_cursor_hidden_ = true;
}

void InputHandler::ShowCursor(GLFWwindow *window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    is_cursor_hidden_ = false;
    first_mouse_ = true;
}

void InputHandler::AddButtonHoldCallback(int key, KeyCallbackFunction callback)
{
    button_hold_callbacks_[key].push_back(callback);
}

void InputHandler::AddButtonPressCallback(int key, KeyCallbackFunction callback)
{
    button_press_callbacks_[key].push_back(callback);
}

void InputHandler::AddButtonReleaseCallback(int key, KeyCallbackFunction callback)
{
    button_release_callbacks_[key].push_back(callback);
}

void InputHandler::AddKeyPress(int key)
{
    InputHandler::Instance()->currently_pressed_keys_.push_back(key);
}

void InputHandler::RemoveKeyPress(int key)
{
    InputHandler::Instance()->currently_pressed_keys_.remove(key);
}

void InputHandler::AddButtonPress(int button) {
    InputHandler::Instance()->currently_pressed_buttons_.push_back(button);
}

void InputHandler::RemoveButtonPress(int button) {
    InputHandler::Instance()->currently_pressed_buttons_.remove(button);
}

bool InputHandler::IsCursorHidden()
{
    return is_cursor_hidden_;
}

