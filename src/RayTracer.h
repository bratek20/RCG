#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "Color.h"
#include "Mesh.h"
#include "Light.h"
#include <vector>

class RayTracer{
public:
    static const float EPSILON;
    static std::pair<bool, Color> cast(int k, float minDist, glm::vec3 origin, glm::vec3 direction, const std::vector<Triangle>& triangles, const std::vector<LightPtr>& lights);
private:
    static Color phongShading(glm::vec3 position, glm::vec3 normal, glm::vec3 reflect, const Material& material, const std::vector<Triangle>& triangles, const std::vector<LightPtr> &lights);
};

#endif