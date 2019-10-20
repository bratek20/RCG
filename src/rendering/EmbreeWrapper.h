#ifndef EMBREE_WRAPPER_H
#define EMBREE_WRAPPER_H

#include "AccStruct.h"
#include "Mesh.h"

#include <vector>
#include <embree3/rtcore.h>

class EmbreeWrapper : public AccStruct {
    const std::vector<Mesh>& meshes;

    RTCDevice device;
    RTCScene scene;

public:
    EmbreeWrapper(const std::vector<Mesh>& meshes);
    ~EmbreeWrapper() override;

    HitData cast(Ray r, float maxDistance) override;

private:
    void createMeshGeometry(const Mesh& mesh);
};

#endif