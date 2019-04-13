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

struct CastData {
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
    const vector<TrianglePtr>& triangles;

public:
    AccStruct(const vector<TrianglePtr>& triangles);
    virtual CastData cast(Ray r, float maxDistance = Utils::INF) = 0;

    static CastData intersect(Ray r, TrianglePtr tri);
};
#endif