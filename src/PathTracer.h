#ifndef PATH_TRACER_H
#define PATH_TRACER_H

#include "Color.h"
#include "AccStruct.h"

class PathTracer{
public:
    struct CastData {
        bool hit = false;
        glm::vec3 emittance = glm::vec3(0,0,0);
        Color color = Colors::BLACK;
    };
    static CastData cast(int k, Ray r, AccStruct& accStruct);
};

#endif