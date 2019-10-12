#include "PathTracer.h"
#include "Random.h"

#include <cstdio>

using namespace std;

PathTracer::CastData PathTracer::cast(Ray r, int k, AccStruct &accStruct, LightSampler& lightSampler) {
    if (k == 0) {
        return CastData();
    }

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
    + calcDirectLight(hit, accStruct, lightSampler) 
    + calcIndirectLight(hit, k, accStruct, lightSampler);
    return ans;
}

glm::vec3 PathTracer::calcDirectLight(HitData& hit, AccStruct &accStruct, LightSampler& lightSampler){
    auto lightSample = lightSampler.sample();
    glm::vec3 lightPoint = lightSample.point;
    TrianglePtr source = lightSample.source;
    glm::vec3 rayDir = glm::normalize(lightPoint - hit.pos);

    HitData visibleHit = accStruct.cast(Ray(hit.pos, rayDir, true));
    if(!visibleHit.intersects() || visibleHit.triangle != source){
        return glm::vec3(0);
    }

    glm::vec3 BRDF = hit.triangle->mat.diffuse / M_PI;
    
    float cosX = glm::clamp(glm::dot(rayDir, hit.triangle->getNormal()), 0.0f, 1.0f);
    float cosY = glm::clamp(glm::dot(-rayDir, source->getNormal()), 0.0f, 1.0f);
    float dist = glm::distance(lightPoint, hit.pos);
    float probability = lightSample.probability / source->calcArea();
    float G = cosX * cosY / (dist * dist);
    auto ans = source->mat.emissive * BRDF * G / probability;
    return ans;
}

glm::vec3 PathTracer::calcIndirectLight(HitData& hit, int k, AccStruct &accStruct, LightSampler& lightSampler){
    glm::vec3 hitNormal = hit.triangle->getNormal(); 
    const Material& hitMat = hit.triangle->mat;

    glm::vec3 newDir = Random::vectorOnHemisphereCos(hitNormal);
    Ray newR = Ray(hit.pos, newDir, true);

    CastData incoming = cast(newR, k-1, accStruct, lightSampler);
    HitData incomingHit = accStruct.cast(newR);
    bool hitLight = incomingHit.intersects() && incomingHit.triangle->mat.isLightSource(); 
    auto ans = hitLight ? glm::vec3(0) : hitMat.diffuse.asVec3() * incoming.emittance;
    return ans;
}

