#include "Input.h"

bool Input::isKeyPressed[Input::KEY_PRESSED_SIZE];
std::function<void()> Input::pressedCallback[Input::KEY_PRESSED_SIZE];
glm::vec2 Input::mouseOffset;
const float Input::MOUSE_SENSIVITY = 20;

void Input::init(){
    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(Window::getPtr(), GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(Window::getPtr(), onKeyClicked);
    glfwSetCursorPosCallback(Window::getPtr(), onMouseMoved);
    glfwSetMouseButtonCallback(Window::getPtr(), onMouseClicked);
    glfwSetInputMode(Window::getPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    onKeyPressed(GLFW_KEY_ESCAPE, [](){glfwSetWindowShouldClose(Window::getPtr(), GL_TRUE);});
}

void Input::handle(){
    mouseOffset = glm::vec2(0,0);
    glfwPollEvents();
}

void Input::onKeyPressed(int key, std::function<void()> callback){
    if(checkRange(key)){
        pressedCallback[key] = callback;
    }
}

bool Input::isPressed(int key){
    return checkRange(key) && isKeyPressed[key];
}

glm::vec2 Input::getMouseOffset(){
    return mouseOffset;
}

bool Input::checkRange(int key){
    return 0 <= key && key < KEY_PRESSED_SIZE;
}

void Input::onKeyClicked(GLFWwindow*, int key, int, int action, int){
    if (!checkRange(key)){
        return;
    }

    bool firstPressed = !isKeyPressed[key] && action == GLFW_PRESS; 
    if (action == GLFW_PRESS){
        isKeyPressed[key] = true;
    }
    else if (action == GLFW_RELEASE){
        isKeyPressed[key] = false;
    }
    
    if(firstPressed && pressedCallback[key])
    {
        pressedCallback[key]();
    }
}

void Input::onMouseMoved(GLFWwindow*, double xpos, double ypos){
    static bool firstMouseMove = true;
    static double lastX = 0.0;
    static double lastY = 0.0;

    if (firstMouseMove){
        lastX = xpos;
        lastY = ypos;
        firstMouseMove = 0;
    }

    mouseOffset.x = xpos - lastX;
    mouseOffset.y = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;
	
    mouseOffset *= MOUSE_SENSIVITY;
}

void Input::onMouseClicked(GLFWwindow*, int button, int action, int){
    if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
        
    }
}