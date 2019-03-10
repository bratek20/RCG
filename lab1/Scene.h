#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Light.h"
#include "Config.h"
#include "Model.h"

class Scene;
using ScenePtr = std::shared_ptr<Scene>;

class Scene : public Actor {
    CameraPtr camera;
    Model model;
    std::vector<LightPtr> lights;

public:
    static ScenePtr create(const Config& c);
    void render();

    CameraPtr getCamera() const;

protected:
    Scene();
};
#endif