#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Light.h"

class Scene;
using ScenePtr = std::shared_ptr<Scene>;

class Scene : public Actor {
    CameraPtr camera;
    LightPtr light;

public:
    static ScenePtr create();
    void render();

    CameraPtr getCamera() const;

protected:
    Scene();
};
#endif