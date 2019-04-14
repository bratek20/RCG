#include "SimpleAccStruct.h"
#include "Globals.h"

using namespace std;

SimpleAccStruct::SimpleAccStruct(const vector<TrianglePtr>& triangles) : AccStruct(triangles) {}

CastData SimpleAccStruct::cast(Ray r, float maxDistance){
    CastData ans;
    ans.distance = Globals::INF;
    
    for (auto tri : triangles) {
        CastData data = intersect(r, tri);
        if (data.intersects() && data.distance < ans.distance && data.distance > 0 && data.distance < maxDistance) {
            ans = data;
        }
    }
    return ans;
}