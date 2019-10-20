#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <glm/glm.hpp>
#include <memory>

class Triangle;
using TrianglePtr = const Triangle*;

class Random {
    static std::default_random_engine generator;
public:
    static float uniform(float a = 0.0f, float b = 1.0f);
    static bool tossCoin(float winProbability);
    static glm::vec3 vectorOnHemisphereUniform(glm::vec3 normal = glm::vec3(0, 1, 0));
    static glm::vec3 vectorOnHemisphereCos(glm::vec3 normal = glm::vec3(0, 1, 0));
    static glm::vec3 pointInTriangle(TrianglePtr triangle);

    static glm::vec3 rotateToGlobalSpace(glm::vec3 vec, glm::vec3 globalNormal);
};
#endif