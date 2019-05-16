#include "PathTracer.h"
#include "Random.h"

#include <cstdio>

using namespace std;

PathTracer::CastData PathTracer::cast(Ray r, AccStruct &accStruct, LightSampler& lightSampler) {
    HitData hit = accStruct.cast(r);
    if (!hit.intersects()) {
        return CastData();
    }
    bool isBounced = Random::tossCoin(hit.triangle->mat.diffuse.getAverage());
    if(!isBounced){
        return CastData();
    }

    CastData ans;
    ans.hit = true;
    ans.emittance = hit.triangle->mat.emissive 
    + randomPart(hit, accStruct, lightSampler) 
    + lightPart(hit, accStruct, lightSampler);
    return ans;
}

glm::vec3 PathTracer::randomPart(HitData& hit, AccStruct &accStruct, LightSampler& lightSampler){
    glm::vec3 hitNormal = hit.triangle->getNormal(); 
    const Material& hitMat = hit.triangle->mat;

    glm::vec3 newDir = Random::vectorOnHemisphereCos(hitNormal);
    Ray newR = Ray(hit.pos, newDir, true);

    CastData incoming = cast(newR, accStruct, lightSampler);
    HitData incomingHit = accStruct.cast(newR);
    bool hitLight = incomingHit.intersects() && incomingHit.triangle->mat.isLightSource(); 
    return  hitLight ? glm::vec3(0) : hitMat.diffuse.asVec3() * incoming.emittance;
}

glm::vec3 PathTracer::lightPart(HitData& hit, AccStruct &accStruct, LightSampler& lightSampler){
    auto lightSample = lightSampler.sample();
    glm::vec3 lightPoint = lightSample.point;
    TrianglePtr source = lightSample.source;
    glm::vec3 rayDir = glm::normalize(lightPoint - hit.pos);

    HitData visibleHit = accStruct.cast(Ray(hit.pos, rayDir, true));
    if(!visibleHit.intersects() || visibleHit.triangle != source){
        return glm::vec3(0);
    }

    glm::vec3 BRDF = hit.triangle->mat.diffuse / M_PI;
    
    float cosX = glm::dot(rayDir, hit.triangle->getNormal());
    float cosY = glm::dot(-rayDir, source->getNormal());
    float dist = glm::distance(lightPoint, hit.pos);
    float probability = 1 / source->calcArea();
    float G = cosX * cosY / (dist * dist);
    return source->mat.emissive * BRDF * G / probability;
}