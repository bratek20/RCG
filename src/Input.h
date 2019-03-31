#ifndef INPUT_H
#define INPUT_H

#include "Window.h"
#include "glm/glm.hpp"

#include <functional>

class Input{
    static const int KEY_PRESSED_SIZE = 1024;
    static bool isKeyPressed[KEY_PRESSED_SIZE];
    static std::function<void()> pressedCallback[KEY_PRESSED_SIZE];
    static glm::vec2 mouseOffset;

public:
    static const float MOUSE_SENSIVITY;

    static void init();
    static void handle();

    static void onKeyPressed(int key, std::function<void()> callback);
    static bool isPressed(int key);
    static glm::vec2 getMouseOffset();

private:
    static bool checkRange(int key);
    static void onKeyClicked(GLFWwindow*, int key, int , int action, int);
    static void onMouseMoved(GLFWwindow*, double xpos, double ypos);
    static void onMouseClicked(GLFWwindow*, int button, int action, int mods);
};
#endif