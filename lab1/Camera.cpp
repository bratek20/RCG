#include "Camera.h"
#include "Globals.h"
#include "Input.h"

#include <algorithm>

using namespace std;

Camera::Camera() : Actor(nullptr) {
    lookPoint = Actor::create(nullptr);
    leftTop = Actor::create(nullptr);
    leftBottom = Actor::create(nullptr);
    rightTop = Actor::create(nullptr);
    rightBottom = Actor::create(nullptr);
}

CameraPtr Camera::create(const CameraConfig &c) {
    CameraPtr camera = CameraPtr(new Camera());
    camera->addChilds({camera->lookPoint, camera->leftTop, camera->leftBottom,
                       camera->rightTop, camera->rightBottom});
    camera->setPosition(c.viewPoint);

    float xView = Window::getRatio() * c.yView;
    camera->lookPoint->setPosition(c.lookAt - c.viewPoint);
    camera->up = c.up;
    camera->setCornerPoints(xView, c.yView,
                            camera->lookPoint->getLocalPosition(), camera->up);
    camera->velocity = c.velocity;
    return camera;
}

void Camera::onUpdate() {
    float stepVal = velocity * Globals::deltaTime;
    glm::vec3 step = glm::vec3(0);
    glm::vec3 front = glm::normalize(lookPoint->getLocalPosition());
    glm::vec3 right = glm::cross(front, up);
    if (Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)) {
        step = front * stepVal;
    }
    if (Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)) {
        step = front * (-stepVal);
    }
    if (Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)) {
        step = right * (-stepVal);
    }
    if (Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)) {
        step = right * stepVal;
    }

    auto newRot = getRotation() +
                  glm::vec3(Input::getMouseOffset().y * Globals::deltaTime,
                            -Input::getMouseOffset().x * Globals::deltaTime, 0);
    static const float MAX_X_DEG = 66;
    newRot.x = min(newRot.x, MAX_X_DEG);
    newRot.x = max(newRot.x, -MAX_X_DEG);
    setRotation(newRot);

    step = static_cast<glm::vec3>(getRotationMat() * glm::vec4(step, 0));
    move(step);
}

glm::mat4 Camera::getViewMat() {
    return glm::lookAt(
        getWorldPosition(), // the position of your camera, in world space
        lookPoint
            ->getWorldPosition(), // where you want to look at, in world space
        up // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking
           // upside-down, which can be great too
    );
}

glm::mat4 Camera::getProjectionMat() {
    float fov = 45;
    return glm::perspective(
        glm::radians(fov), // The vertical Field of View, in radians: the amount
                           // of "zoom". Think "camera lens". Usually between
                           // 90° (extra wide) and 30° (quite zoomed in)
        Window::getRatio(), // Aspect Ratio. Depends on the size of your window.
                            // Notice that 4/3 == 800/600 == 1280/960, sounds
                            // familiar ?
        0.1f,  // Near clipping plane. Keep as big as possible, or you'll get
               // precision issues.
        2000.0f // Far clipping plane. Keep as little as possible.
    );
}

void Camera::setCornerPoints(float xView, float yView, glm::vec3 lookDir,
                             glm::vec3 up) {
    static const float DEPTH = 2;
    glm::vec3 front = glm::normalize(lookDir);
    glm::vec3 right = glm::normalize(glm::cross(front, up));
    glm::vec3 top = glm::normalize(glm::cross(right, front));

    front *= DEPTH;
    right *= xView;
    top *= yView;

    leftTop->setPosition(front - right + top);
    leftBottom->setPosition(front - right - top);
    rightTop->setPosition(front + right + top);
    rightBottom->setPosition(front - right - top);
}

glm::vec3 Camera::getLeftTop() const { return leftTop->getWorldPosition(); }

glm::vec3 Camera::getLeftBottom() const {
    return leftBottom->getWorldPosition();
}

glm::vec3 Camera::getRightTop() const { return rightTop->getWorldPosition(); }

glm::vec3 Camera::getRightBottom() const {
    return rightBottom->getWorldPosition();
}