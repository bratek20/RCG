#include "AccStruct.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

using namespace std;

float Ray::epsilon = 1e-3;

Ray::Ray(glm::vec3 origin, glm::vec3 direction, bool moveByEpsilon) : 
        origin(origin), direction(glm::normalize(direction)) {
    if(moveByEpsilon){
        this->origin += this->direction * epsilon;
    }
}

void Ray::setEpsilon(float eps){
    epsilon = eps;
    cout << "Ray epsilon set to: " << eps << endl;
}

TrianglesAccStruct::TrianglesAccStruct(const vector<TrianglePtr>& triangles) : triangles(triangles) {}

HitData TrianglesAccStruct::intersect(Ray r, TrianglePtr tri){
    HitData data;
    bool intersects = glm::intersectRayTriangle(
        r.origin, r.direction, tri->v1.position, tri->v2.position,
        tri->v3.position, data.baryPos, data.distance);
    
    if(intersects){
        data.triangle = tri;
        data.pos = r.origin + r.direction * data.distance;
    }
     
    return data;
}