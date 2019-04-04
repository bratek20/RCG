#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "Color.h"
#include "AccStruct.h"
#include "Light.h"
#include <vector>

class RayTracer{
public:
    static std::pair<bool, Color> cast(int k, Ray r, AccStruct& accStruct, const std::vector<LightPtr>& lights);
private:
    static Color phongShading(glm::vec3 position, glm::vec3 normal, glm::vec3 reflect, const Material& material, AccStruct& accStruct, const std::vector<LightPtr> &lights);
};

#endif