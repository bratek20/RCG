#include "RayTracer.h"

#include <cstdio>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

const float RayTracer::EPSILON = 1e-3;

std::pair<bool, Color> RayTracer::cast(int k, float minDist, glm::vec3 origin,
                                       glm::vec3 direction,
                                       const std::vector<Triangle> &triangles,
                                       const std::vector<LightPtr> &lights) {
    const Triangle *ansTri = NULL;
    float ansDist = std::numeric_limits<float>::max();
    glm::vec2 ansBaryPos;

    for (auto &tri : triangles) {
        glm::vec2 baryPos;
        float distance;
        bool intersetcs = glm::intersectRayTriangle(
            origin, direction, tri.v1.position, tri.v2.position,
            tri.v3.position, baryPos, distance);
        if (intersetcs && distance < ansDist && distance > minDist) {
            ansTri = &tri;
            ansDist = distance;
            ansBaryPos = baryPos;
        }
    }

    if (ansTri == NULL) {
        return std::make_pair(false, Colors::BLACK);
    }

    if (k == 0) {
        return std::make_pair(true, ansTri->mat.diffuse);
    }

    glm::vec3 intersec = origin + direction * ansDist;
    glm::vec3 normal = ansTri->getNormal(ansBaryPos);
    glm::vec3 reflectDir =
        glm::normalize(glm::reflect(glm::normalize(intersec), normal));
    float reflectParam = ansTri->mat.specular.getAverage();
    pair<bool, Color> reflectCast =
        cast(k - 1, EPSILON, intersec, reflectDir, triangles, lights);
    Color phongColor = phongShading(intersec, normal, direction,
                                    ansTri->mat, triangles, lights);
    return std::make_pair(true, phongColor + reflectCast.second * reflectParam);
}

Color RayTracer::phongShading(glm::vec3 position, glm::vec3 normal,
                              glm::vec3 rayDirection, const Material &material,
                              const std::vector<Triangle> &triangles,
                              const std::vector<LightPtr> &lights) {
    Color color = Colors::BLACK;
    Color materialDiffuseColor = material.diffuse;
    Color materialAmbientColor = material.ambient * materialDiffuseColor;
    Color materialSpecularColor = material.specular;
    for (auto &light : lights) {
        // Distance to the light
        glm::vec3 lightDir = light->getWorldPosition() - position;
        float distance = length(lightDir);
        lightDir = glm::normalize(lightDir);
        pair<bool, Color> hit =
            cast(0, EPSILON, position, lightDir, triangles, lights);
        if (hit.first) {
            continue;
        }

        // Cosine of the angle between the normal and the light direction,
        // clamped above 0
        //  - light is at the vertical of the triangle -> 1
        //  - light is perpendicular to the triangle -> 0
        //  - light is behind the triangle -> 0
        glm::vec3 l = glm::normalize(lightDir);
        float cosTheta = glm::clamp(glm::dot(normal, l), 0.0f, 1.0f);

        // Eye vector (towards the camera)
        // Direction in which the triangle reflects the light
        glm::vec3 reflectedL = glm::normalize(reflect(-l, normal));
        // Cosine of the angle between the Eye vector and the Reflect vector,
        // clamped to 0
        //  - Looking into the reflection -> 1
        //  - Looking elsewhere -> < 1
        float cosAlpha = glm::clamp(
            glm::dot(normalize(-rayDirection), reflectedL), 0.0f, 1.0f);
        auto coeff = light->getCoefficients();
        float distanceLoss =
            distance * distance * coeff.x + distance * coeff.y + coeff.z;

        color +=
            // Ambient : simulates indirect lighting
            materialAmbientColor
            // Diffuse : "color" of the object
            + materialDiffuseColor * light->getColor() * light->getPower() *
                  cosTheta / distanceLoss
            // Specular : reflective highlight, like a mirror
            + materialSpecularColor * light->getColor() * light->getPower() *
                  pow(cosAlpha, material.ns) / distanceLoss;
    }
    return color;
}