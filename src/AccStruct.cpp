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

AccStruct::AccStruct(const vector<TrianglePtr>& triangles) : triangles(triangles) {}

bool AccStruct::isVisible(glm::vec3 p1, glm::vec3 p2){
    Ray r(p1, (p2-p1).normalized());
    HitData data = cast(r, (p2-p1).length());
    return !data.intersects();
}

HitData AccStruct::intersect(Ray r, TrianglePtr tri){
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