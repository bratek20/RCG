#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "Color.h"
#include "Mesh.h"
#include <vector>

class RayTracer{
public:
    static Color cast(glm::vec3 origin, glm::vec3 direction, const std::vector<Triangle>& triangles);
};

#endif