#ifndef PATH_TRACER_H
#define PATH_TRACER_H

#include "Color.h"
#include "AccStruct.h"
#include "LightSampler.h"

class PathTracer{
public:
    struct CastData {
        bool hit = false;
        glm::vec3 emittance = glm::vec3(0,0,0);
    };
    static CastData cast(Ray r, int k, AccStruct& accStruct, LightSampler& lightSampler);

private:
    static glm::vec3 calcDirectLight(HitData& hit, AccStruct &accStruct, LightSampler& lightSampler);
    static glm::vec3 calcIndirectLight(HitData& hit, int k, AccStruct &accStruct, LightSampler& lightSampler);
};

#endif