#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.h"

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Camera : public Actor {
public:
    static const glm::vec3 LOCAL_UP;

    static CameraPtr create();
    glm::mat4 getViewMat();
    glm::mat4 getProjectionMat();

protected:
    Camera();

    void onUpdate() override;
    
private:
    ActorPtr lookPoint;
};

#endif