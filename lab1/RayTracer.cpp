#include "RayTracer.h"

#include <cstdio>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

Color RayTracer::cast(glm::vec3 origin, glm::vec3 direction, const std::vector<Triangle> &triangles)
{
    const Triangle *ansTri = NULL;
    float ansDist = std::numeric_limits<float>::max();
    glm::vec2 ansBaryPos;

    for (auto &tri : triangles)
    {
        glm::vec2 baryPos;
        float distance;
        bool intersetcs = glm::intersectRayTriangle(origin, direction, tri.v1.position, tri.v2.position, tri.v3.position, baryPos, distance); 
        if (intersetcs && distance < ansDist)
        {
            ansTri = &tri;
            ansDist = distance;
            ansBaryPos = baryPos;
        }
    }

    if (ansTri == NULL)
    {
        return Colors::BLACK;
    }
    return ansTri->v1.color;
}