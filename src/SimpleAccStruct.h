#ifndef SIMPLE_ACC_STRUCT_H
#define SIMPLE_ACC_STRUCT_H

#include "AccStruct.h"

class SimpleAccStruct : public AccStruct {

public:
    SimpleAccStruct(const std::vector<TrianglePtr>& triangles);
    HitData cast(Ray r, float maxDistance) override;
};
#endif