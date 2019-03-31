#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.h"
#include "Config.h"

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Camera : public Actor {
    ActorPtr lookPoint;
    ActorPtr leftTop;
    ActorPtr leftBottom;
    ActorPtr rightTop;
    ActorPtr rightBottom;
    glm::vec3 up;
    float velocity;

public:
    static CameraPtr create(const CameraConfig& c);
    
    glm::mat4 getViewMat();
    glm::mat4 getProjectionMat();
    
    glm::vec3 getLeftTop() const;
    glm::vec3 getLeftBottom() const;
    glm::vec3 getRightTop() const;
    glm::vec3 getRightBottom() const;

protected:
    Camera();

    void onUpdate() override;

private:
    void setCornerPoints(float xView, float yView, glm::vec3 lookDir, glm::vec3 up);

};

#endif