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
    static InputHandler *inputHandler;

    std::unordered_map<int, std::list<KeyCallbackFunction>> keyHoldCallbacks;
    std::unordered_map<int, std::list<KeyCallbackFunction>> keyPressCallbacks;
    std::unordered_map<int, std::list<KeyCallbackFunction>> keyReleaseCallbacks;

    std::unordered_map<int, std::list<KeyCallbackFunction>> buttonHoldCallbacks;
    std::unordered_map<int, std::list<KeyCallbackFunction>> buttonPressCallbacks;
    std::unordered_map<int, std::list<KeyCallbackFunction>> buttonReleaseCallbacks;

    std::list<MouseCallbackFunction> mouseCallbacks;

    std::list<int> currentlyPressedKeys{};
    std::list<int> currentlyPressedButtons{};

    // Mouse Input
    
    bool firstMouse = true;
    float lastX = 0;
    float lastY = 0;

    bool bIsCursorHidden = false;


public:

    

    InputHandler(InputHandler &other) = delete;

    void operator=(const InputHandler &) = delete;

    static InputHandler *instance();

    void addKeyHoldCallback(int key, KeyCallbackFunction callback);

    void addKeyPressCallback(int key, KeyCallbackFunction callback);

    void addKeyReleaseCallback(int key, KeyCallbackFunction callback);

    void addButtonHoldCallback(int key, KeyCallbackFunction callback);

    void addButtonPressCallback(int key, KeyCallbackFunction callback);
    
    void addButtonReleaseCallback(int key, KeyCallbackFunction callback);
    
    void addMouseMoveCallback(MouseCallbackFunction callback);

    void calcMouseOffset(double x, double y, double &xOffset, double &yOffset);

    static void registerKeys(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void registerButtons(GLFWwindow *window, int button, int action, int mods);

    static void processMouseInput(GLFWwindow *window, double xpos, double ypos);

    void hideCursor(GLFWwindow *window);

    void showCursor(GLFWwindow *window);

    void processInput();

    void addKeyPress(int key);

    void removeKeyPress(int key);

    void addButtonPress(int button);

    void removeButtonPress(int button);

    void updateMousePos(GLFWwindow *window, double xPos, double yPos);

    bool isCursorHidden();

};

#endif // INPUT_HANDLER_H