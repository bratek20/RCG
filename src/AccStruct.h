#ifndef ACC_STRUCT_H
#define ACC_STRUCT_H

#include "Mesh.h"

struct Ray {
    static float epsilon;

    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3 origin, glm::vec3 direction, bool moveByEpsilon = false);
    static void setEpsilon(float epsilon);
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
protected:
    const std::vector<TrianglePtr>& triangles;

public:
    AccStruct(const std::vector<TrianglePtr>& triangles);
    virtual HitData cast(Ray r, float maxDistance = Utils::INF) = 0;

    static HitData intersect(Ray r, TrianglePtr tri);
};
#endif