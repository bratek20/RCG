#include "RayTracer.h"

#include <cstdio>

using namespace std;

pair<bool, Color> RayTracer::cast(int k, Ray r, AccStruct& accStruct,
                                       const vector<LightPtr> &lights) {
    CastData ans = accStruct.cast(r);
    if (!ans.intersects()) {
        return make_pair(false, Colors::BLACK);
    }

    TrianglePtr ansTri = ans.triangle; 
    if (k == 0) {
        return make_pair(true, ansTri->mat.diffuse);
    }

    glm::vec3 intersec = ans.pos;
    glm::vec3 normal = ansTri->getNormal(ans.baryPos);
    glm::vec3 reflectDir =
        glm::normalize(glm::reflect(glm::normalize(intersec), normal));
    float reflectParam = ansTri->mat.specular.getAverage();
    pair<bool, Color> reflectCast =
        cast(k - 1, Ray(intersec, reflectDir, true), accStruct, lights);
    Color phongColor = phongShading(intersec, normal, r.direction,
                                    ansTri->mat, accStruct, lights);
    return make_pair(true, phongColor + reflectCast.second * reflectParam);
}

Color RayTracer::phongShading(glm::vec3 position, glm::vec3 normal,
                              glm::vec3 rayDirection, const Material &material,
                              AccStruct& accStruct,
                              const vector<LightPtr> &lights) {
    Color color = Colors::BLACK;
    Color materialDiffuseColor = material.diffuse;
    Color materialAmbientColor = material.ambient * materialDiffuseColor;
    Color materialSpecularColor = material.specular;
    for (auto &light : lights) {
        // Distance to the light
        glm::vec3 lightDir = light->getWorldPosition() - position;
        float distance = length(lightDir);
        lightDir = glm::normalize(lightDir);
        CastData hit = accStruct.cast(Ray(position, lightDir, true), distance - Ray::epsilon);
        if (hit.intersects()) {
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