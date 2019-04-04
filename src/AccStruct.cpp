#include "AccStruct.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

float Ray::EPSILON = 1e-3;

Ray::Ray(glm::vec3 origin, glm::vec3 direction, bool moveByEpsilon) : 
        origin(origin), direction(glm::normalize(direction)) {
    if(moveByEpsilon){
        this->origin += this->direction * EPSILON;
    }
}

AccStruct::AccStruct(const vector<TrianglePtr>& triangles) : triangles(triangles) {}

CastData AccStruct::intersect(Ray r, TrianglePtr tri){
    CastData data;
    bool intersects = glm::intersectRayTriangle(
        r.origin, r.direction, tri->v1.position, tri->v2.position,
        tri->v3.position, data.baryPos, data.distance);
    
    if(intersects){
        data.triangle = tri;
        data.pos = r.origin + r.direction * data.distance;
    }
     
    return data;
}