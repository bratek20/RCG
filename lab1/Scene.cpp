#include "Scene.h"
#include "Color.h"

Scene::Scene() : Actor(nullptr) {
    camera = Camera::create();

    light = Light::create(0.3f, Colors::WHITE, {0, 0, 1});
    light->move({10, 100, 20});
}

ScenePtr Scene::create(){
    ScenePtr scene = ScenePtr(new Scene());
    scene->addChilds({scene->camera, scene->light});
    return scene; 
}

void Scene::render(){
    Mesh::setViewMat(camera->getViewMat());
    Mesh::setProjectionMat(camera->getProjectionMat());
    Mesh::applyLights();
    Mesh::applyPlayerPosition(camera->getWorldPosition());
    Actor::render(glm::mat4(1.0f));
}

CameraPtr Scene::getCamera() const{
    return camera;
}