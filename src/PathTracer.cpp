#include "PathTracer.h"
#include "Random.h"

#include <cstdio>

using namespace std;

PathTracer::CastData PathTracer::cast(int k, Ray r, AccStruct &accStruct) {
    if(k < 0){
        return CastData();
    }
    HitData hit = accStruct.cast(r);
    if (!hit.intersects()) {
        return CastData();
    }

    glm::vec3 hitNormal = hit.triangle->getNormal(); 
    const Material& mat = hit.triangle->mat;
    
    glm::vec3 newDir = Random::vectorOnHemisphere(hitNormal);
    Ray newR = Ray(hit.pos, newDir, true);
    float rayP = 1 / (2*M_PI);
    
    Color BRDF = mat.specular / M_PI;
    float cosTheta = glm::dot(newR.direction, hitNormal);

    CastData incoming = cast(k-1, newR, accStruct);
    CastData ans;
    ans.hit = true;
    ans.color = mat.diffuse + incoming.color * BRDF * cosTheta / rayP;
    ans.emittance = mat.emissive + incoming.emittance * static_cast<glm::vec3>(BRDF) * cosTheta / rayP;
    return ans;
}