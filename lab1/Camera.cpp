#include "Camera.h"
#include "Globals.h"
#include "Input.h"

#include <algorithm>

using namespace std;

Camera::Camera() : Actor(nullptr){
    lookPoint = Actor::create(nullptr);
    lookPoint->move({0,0,1});
    up = glm::vec3(0,1,0);
}

CameraPtr Camera::create(const Config& c){
    CameraPtr camera = CameraPtr(new Camera());
    camera->addChild(camera->lookPoint);
    camera->setPosition(c.viewPoint);
    camera->lookPoint->setPosition(c.lookAt - c.viewPoint);
    camera->up = c.up;
    return camera;
}

void Camera::onUpdate(){
    static const float VELOCITY = 1;
    float stepVal = VELOCITY * Globals::deltaTime;
    glm::vec3 step = glm::vec3(0);
    glm::vec3 front = lookPoint->getLocalPosition();
    glm::vec3 right = glm::cross(front, up);
    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        step = front * stepVal;
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        step = front * (-stepVal);
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        step = right * (-stepVal);
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        step = right * stepVal;
    }

    auto newRot = getRotation() + glm::vec3(Input::getMouseOffset().y * Globals::deltaTime, -Input::getMouseOffset().x * Globals::deltaTime, 0); 
    static const float MAX_X_DEG = 66;
    newRot.x = min(newRot.x, MAX_X_DEG);
    newRot.x = max(newRot.x, -MAX_X_DEG);
    setRotation(newRot);

    step = static_cast<glm::vec3>(getRotationMat() * glm::vec4(step, 0));
    move(step);    
}

glm::mat4 Camera::getViewMat(){
    return glm::lookAt(
                getWorldPosition(), // the position of your camera, in world space
                lookPoint->getWorldPosition(),   // where you want to look at, in world space
                up       // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
            );
}

glm::mat4 Camera::getProjectionMat(){
    float fov = 53;
    return glm::perspective(
                glm::radians(fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
                1 / Window::getRatio(),       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
                0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
                400.0f             // Far clipping plane. Keep as little as possible.
            );
}