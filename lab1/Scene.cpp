#include "Scene.h"
#include "Color.h"
#include "RayTracer.h"

#include <bitmap_image.h>

Scene::Scene() : Actor(nullptr)
{
}

ScenePtr Scene::create(const Config &c)
{
    ScenePtr scene = ScenePtr(new Scene());
    scene->camera = Camera::create(c.camera);
    scene->addChild(scene->camera);
    //auto a = Actor::create(MyMesh::create(Assets::CUBE, Colors::BLACK));
    //scene->addChild(a);
    scene->model = Model(c.loadScenePath);
    Light::loadLights(c);
    return scene;
}

void Scene::render()
{
    MyMesh::setViewMat(camera->getViewMat());
    MyMesh::setProjectionMat(camera->getProjectionMat());
    MyMesh::applyPlayerPosition(camera->getWorldPosition());
    Actor::render(glm::mat4(1.0f));
    MyMesh::program.applyWorldMat(glm::mat4(1.0f));
    Light::applyLights(MyMesh::program);
    model.draw(MyMesh::program);
}

CameraPtr Scene::getCamera() const
{
    return camera;
}

void Scene::takePhoto(const Config &c)
{
    cout << "Taking photo..." << endl;
    bitmap_image photo(c.xRes, c.yRes);

    glm::vec3 origin = camera->getWorldPosition();
    cout << "Camera position: " << origin.x  << ", " << origin.y << ", " <<origin.z << endl;
    vector<Triangle> triangles = model.getTriangles();
    cout << "Triangles number: " << triangles.size() << endl;

    glm::vec3 leftTop = camera->getLeftTop();
    glm::vec3 leftBottom = camera->getLeftBottom();
    glm::vec3 rightTop = camera->getRightTop();
    for (int x = 0; x < c.xRes; x++)
    {
        for (int y = 0; y < c.yRes; y++)
        {
            float xShift = static_cast<float>(x) / c.xRes;
            float yShift = static_cast<float>(y) / c.yRes;
            glm::vec3 pos = -leftTop + glm::mix(leftTop, rightTop, xShift) + glm::mix(leftTop, leftBottom, yShift); 
            glm::vec3 direction = glm::normalize(pos - origin);
            auto hit = RayTracer::cast(c.k, 0, origin, direction, triangles, Light::getLights());
            Color color = hit.first ? hit.second : c.background;
            photo.set_pixel(x, y, color.r * 255, color.g * 255, color.b * 255);
        }
    }

    photo.save_image(c.saveImagePath);
    cout << "Photo taken and saved to: " << c.saveImagePath << endl;
}