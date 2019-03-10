#include "Scene.h"
#include "Color.h"

Scene::Scene() : Actor(nullptr) {
    camera = Camera::create();

    light = Light::create(3.0f, Colors::WHITE, {0, 0, 1});
    light->move({100, 1000, 200});
}

ScenePtr Scene::create(){
    ScenePtr scene = ScenePtr(new Scene());
    scene->addChilds({scene->camera, scene->light});
    return scene; 
}

void Scene::render(){
    MyMesh::setViewMat(camera->getViewMat());
    MyMesh::setProjectionMat(camera->getProjectionMat());
    MyMesh::applyLights();
    MyMesh::applyPlayerPosition(camera->getWorldPosition());
    Actor::render(glm::mat4(1.0f));
}

CameraPtr Scene::getCamera() const{
    return camera;
}