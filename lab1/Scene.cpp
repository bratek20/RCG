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
    scene->camera = Camera::create(c);
    scene->addChild(scene->camera);
    scene->model = Model(c.loadScenePath);
    return scene;
}

void Scene::render()
{
    MyMesh::setViewMat(camera->getViewMat());
    MyMesh::setProjectionMat(camera->getProjectionMat());
    MyMesh::applyLights();
    MyMesh::applyPlayerPosition(camera->getWorldPosition());
    Actor::render(glm::mat4(1.0f));
    MyMesh::program.applyWorldMat(glm::mat4(1.0f));
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
    vector<Triangle> triangles = model.getTriangles();
    // auto printVec3 = [](glm::vec3 v){cout << "(" << v.x << ", " << v.y << ", " << v.z << ") ";};
    // auto printV = [&](Vertex v){printVec3(v.position);};
    // for(auto& tri : triangles){
    //     printV(tri.v1);
    //     printV(tri.v2);
    //     printV(tri.v3);
    //     cout << endl;
    // }
    static const glm::vec3 TOP_LEFT = glm::vec3(-1, 1, 0);
    static const glm::vec3 MAX_SHIFT = glm::vec3(2, -2, 0);
    for (int x = 0; x < c.xRes; x++)
    {
        for (int y = 0; y < c.yRes; y++)
        {
            float xRat = static_cast<float>(x) / c.xRes;
            float yRat = static_cast<float>(y) / c.yRes;
            glm::vec3 pos = TOP_LEFT + MAX_SHIFT * glm::vec3(xRat, yRat, 1);
            glm::vec3 direction = glm::normalize(pos - origin);
            Color color = RayTracer::cast(origin, direction, triangles);
            photo.set_pixel(x, y, color.r * 255, color.g * 255, color.b * 255);
        }
    }

    photo.save_image(c.saveImagePath);
    cout << "Photo taken and saved to: " << c.saveImagePath << endl;
}