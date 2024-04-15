#include "InputHandler.h"

#include <GLFW/glfw3.h>
#include "iostream"


InputHandler *InputHandler::inputHandler = nullptr;

InputHandler::InputHandler() {

}

InputHandler *InputHandler::getInstance()
{
    if(!inputHandler) {
        inputHandler = new InputHandler();
    }
    return inputHandler;
}

void InputHandler::addKeyHoldCallback(int key, KeyCallbackFunction callback)
{
    keyHoldCallbacks[key].push_back(callback);
}

void InputHandler::addKeyPressCallback(int key, KeyCallbackFunction callback)
{
    keyHoldCallbacks[key].push_back(callback);
}

void InputHandler::addKeyReleaseCallback(int key, KeyCallbackFunction callback)
{
    keyHoldCallbacks[key].push_back(callback);
}

void InputHandler::addMouseMoveCallback(MouseCallbackFunction callback)
{
    mouseCallbacks.push_back(callback);
}

void InputHandler::registerKeys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        auto it = inputHandler->keyPressCallbacks.find(key);
        if(it != inputHandler->keyPressCallbacks.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }

        InputHandler::getInstance()->addKeyPress(key);
    } else if(action == GLFW_RELEASE) {
        auto it = inputHandler->keyReleaseCallbacks.find(key);
        if(it != inputHandler->keyReleaseCallbacks.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }
        InputHandler::getInstance()->removeKeyPress(key);
    }
}

void InputHandler::registerButtons(GLFWwindow *window, int button, int action, int mods) {
    if(action == GLFW_PRESS) {
        auto it = inputHandler->buttonPressCallbacks.find(button);
        if(it != inputHandler->buttonPressCallbacks.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }

        InputHandler::getInstance()->addButtonPress(button);
    } else if(action == GLFW_RELEASE) {
        auto it = inputHandler->buttonReleaseCallbacks.find(button);
        if(it != inputHandler->buttonReleaseCallbacks.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }

        InputHandler::getInstance()->removeButtonPress(button);
    }
}

void InputHandler::processMouseInput(GLFWwindow *window, double xpos, double ypos)
{
    InputHandler *inputHandler = InputHandler::getInstance();

    if(!inputHandler->bIsCursorHidden) {
        return;
    }

    double xOffset, yOffset;

    inputHandler->calcMouseOffset(xpos, ypos, xOffset, yOffset);


    for(auto callback : InputHandler::getInstance()->mouseCallbacks) {
        callback(xOffset, yOffset);
    }
}

void InputHandler::calcMouseOffset(double x, double y, double &xOffset, double &yOffset)
{
    if(firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    xOffset = x - lastX;
    yOffset = y - lastY;
    lastX = x;
    lastY = y;
}

void InputHandler::processInput() {
    for(auto key : currentlyPressedKeys) {
        auto it = inputHandler->keyHoldCallbacks.find(key);
        if(it != inputHandler->keyHoldCallbacks.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }
    }   

    for(auto button : currentlyPressedButtons) {
        auto it = inputHandler->buttonHoldCallbacks.find(button);
        if(it != inputHandler->buttonHoldCallbacks.end()) {
            for(auto key2 : it->second) {
                key2();
            }
        }
    }
}

void InputHandler::hideCursor(GLFWwindow *window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    bIsCursorHidden = true;
}

void InputHandler::showCursor(GLFWwindow *window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    bIsCursorHidden = false;
    firstMouse = true;
}

void InputHandler::addButtonHoldCallback(int key, KeyCallbackFunction callback)
{
    buttonHoldCallbacks[key].push_back(callback);
}

void InputHandler::addButtonPressCallback(int key, KeyCallbackFunction callback)
{
    buttonPressCallbacks[key].push_back(callback);
}

void InputHandler::addButtonReleaseCallback(int key, KeyCallbackFunction callback)
{
    buttonReleaseCallbacks[key].push_back(callback);
}

void InputHandler::addKeyPress(int key)
{
    InputHandler::getInstance()->currentlyPressedKeys.push_back(key);
}

void InputHandler::removeKeyPress(int key)
{
    InputHandler::getInstance()->currentlyPressedKeys.remove(key);
}

void InputHandler::addButtonPress(int button) {
    InputHandler::getInstance()->currentlyPressedButtons.push_back(button);
}

void InputHandler::removeButtonPress(int button) {
    InputHandler::getInstance()->currentlyPressedButtons.remove(button);
}

bool InputHandler::isCursorHidden()
{
    return bIsCursorHidden;
}

