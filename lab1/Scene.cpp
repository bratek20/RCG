#include "Scene.h"
#include "Color.h"

#include <bitmap_image.h>

Scene::Scene() : Actor(nullptr) {
}

ScenePtr Scene::create(const Config& c){
    ScenePtr scene = ScenePtr(new Scene());
    scene->camera = Camera::create(c);
    scene->addChild(scene->camera);
    scene->model = Model(c.loadScenePath);
    return scene; 
}

void Scene::render(){
    MyMesh::setViewMat(camera->getViewMat());
    MyMesh::setProjectionMat(camera->getProjectionMat());
    MyMesh::applyLights();
    MyMesh::applyPlayerPosition(camera->getWorldPosition());
    Actor::render(glm::mat4(1.0f));
    MyMesh::program.applyWorldMat(glm::mat4(1.0f));
    model.draw(MyMesh::program);
}

CameraPtr Scene::getCamera() const{
    return camera;
}

void Scene::takePhoto(const Config& c){
    cout << "Taking photo..." << endl;
    bitmap_image img(c.xRes, c.yRes);
	img.save_image(c.saveImagePath);
    cout << "Photo taken and saved to: " << c.saveImagePath << endl;
}