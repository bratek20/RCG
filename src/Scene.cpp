#include "Scene.h"
#include "Assets.h"
#include "Color.h"
#include "KDTree.h"
#include "PhotoSaver.h"
#include "RayTracer.h"
#include "SimpleAccStruct.h"
#include "Timer.h"

using namespace std;

Scene::Scene(ModelPtr sceneModel) : Actor(sceneModel) {}

ScenePtr Scene::create(const Config &c) {
    Timer::start("Creating scene");
    string fullScenePath = Assets::validPath(c.loadScenePath);
    ScenePtr scene = ScenePtr(new Scene(Model::create(fullScenePath)));
    scene->camera = Camera::create(c.camera);
    scene->addChild(scene->camera);
    Light::loadLights(c.lights);
    Light::loadLights(scene->getModel()->getLights());
    Timer::stop();
    return scene;
}

void Scene::render() {
    Assets::program.setViewMat(camera->getViewMat());
    Assets::program.setProjectionMat(camera->getProjectionMat());

    Light::applyLights(Assets::program);
    Actor::render(glm::mat4(1.0f));
}

CameraPtr Scene::getCamera() const { return camera; }

void Scene::takePhoto(const Config &c) {
    Ray::setEpsilon(c.rayEpsilon);

    cout << "Taking photo..." << endl;
    PhotoSaver photo(c.xRes, c.yRes);

    glm::vec3 origin = camera->getWorldPosition();
    cout << "Camera position: " << origin.x << ", " << origin.y << ", "
         << origin.z << endl;
    auto &triangles = getModel()->getTriangles();
    cout << "Triangles number: " << triangles.size() << endl;

    Timer::start("Building accStruct");
    KDTree accStruct(triangles);
    Timer::stop();

    Timer::start("Raytracing");
    glm::vec3 leftTop = camera->getLeftTop();
    glm::vec3 leftBottom = camera->getLeftBottom();
    glm::vec3 rightTop = camera->getRightTop();
    for (int x = 0; x < c.xRes; x++) {
        for (int y = 0; y < c.yRes; y++) {
            float xShift = static_cast<float>(x) / c.xRes;
            float yShift = static_cast<float>(y) / c.yRes;
            glm::vec3 pos = -leftTop + glm::mix(leftTop, rightTop, xShift) +
                            glm::mix(leftTop, leftBottom, yShift);
            glm::vec3 direction = glm::normalize(pos - origin);
            auto hit = RayTracer::cast(c.k, Ray(origin, direction), accStruct,
                                       Light::getLights());
            Color color = hit.first ? hit.second : c.background;
            photo.setPixel(x, y, color);
        }

        int progress = 100 * x / c.xRes;
        cout << "\rProgress: " << progress << "%";
    }
    cout << endl;
    Timer::stop();

    string photoSavePath = Assets::photoSavePath(c.photoName);
    photo.save(photoSavePath);
    cout << "Photo taken and saved to: " << photoSavePath << endl;
}