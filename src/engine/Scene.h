#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Light.h"
#include "Config.h"
#include "LightSampler.h"

class Scene;
using ScenePtr = std::shared_ptr<Scene>;

class Scene : public Actor {
    CameraPtr camera;
    LightSampler lightSampler;

public:
    static ScenePtr create(const Config& c);
    void render();

    CameraPtr getCamera() const;
    void takePhotoRayTracing(const Config& c);
    void takePhotoPathTracing(const Config& c);
    void debugRay(const Config& c);

protected:
    Scene(ModelPtr sceneModel);
};
#endif