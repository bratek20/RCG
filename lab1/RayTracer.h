#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "Color.h"
#include "Triangle.h"
#include <vector>

class RayTracer{
public:
    Color cast(glm::vec3 origin, glm::vec3 direction, std::vector<Triangle>& triangles);
};

#endif