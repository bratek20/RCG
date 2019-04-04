#ifndef SIMPLE_ACC_STRUCT_H
#define SIMPLE_ACC_STRUCT_H

#include "AccStruct.h"

class SimpleAccStruct : public AccStruct {

public:
    SimpleAccStruct(const vector<TrianglePtr>& triangles);
    CastData cast(Ray r) override;
};
#endif