#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Color.h"
#include "Config.h"

class Window{
    static GLFWwindow* window;
    static float ratio;

public:
    static bool open(const std::string& name, const Config& c);
    static void clear();
    static void swapBuffers();
    static bool shouldClose();
    static void close();

    static GLFWwindow* getPtr();
    static float getRatio();

private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void setRatio(int width, int height);
};
#endif