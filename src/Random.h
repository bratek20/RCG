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
    static glm::vec3 vectorOnHemisphere(glm::vec3 normal = glm::vec3(0, 1, 0));
    static glm::vec3 pointInTriangle(TrianglePtr triangle);
};
#endif