#ifndef ACC_STRUCT_H
#define ACC_STRUCT_H

#include "Mesh.h"

#include <embree3/rtcore_ray.h>

struct Ray {
    static float epsilon;

    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3 origin, glm::vec3 direction, bool moveByEpsilon = false);
    static void setEpsilon(float epsilon);

    RTCRay toRTCRay(float maxDistance);
};

struct HitData {
    TrianglePtr triangle = nullptr;
    float distance;
    glm::vec3 pos;
    glm::vec2 baryPos;

    bool intersects() const{
        return triangle != nullptr;
    }
};

class AccStruct {
public:
    virtual ~AccStruct(){}
    
    virtual HitData cast(Ray r, float maxDistance = Utils::INF) = 0;
};

class TrianglesAccStruct : public AccStruct {    
protected:
    const std::vector<TrianglePtr>& triangles;

public:
    TrianglesAccStruct(const std::vector<TrianglePtr>& triangles);
    
    bool isVisible(glm::vec3 p1, glm::vec3 p2);
    
    static HitData intersect(Ray r, TrianglePtr tri);
};
#endif