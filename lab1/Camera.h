#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.h"
#include "Config.h"

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Camera : public Actor {
public:
    static CameraPtr create(const Config& c);
    glm::mat4 getViewMat();
    glm::mat4 getProjectionMat();

protected:
    Camera();

    void onUpdate() override;
    
private:
    ActorPtr lookPoint;
    glm::vec3 up;
};

#endif