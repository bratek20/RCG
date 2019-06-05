#include "EmbreeWrapper.h"

using namespace std;

EmbreeWrapper::EmbreeWrapper(const vector<Mesh>& meshes) : meshes(meshes) {
    device = rtcNewDevice(NULL);
    scene = rtcNewScene(device);
    for(auto& mesh : meshes){
        createMeshGeometry(mesh);
    }

    rtcCommitScene(scene);
}

EmbreeWrapper::~EmbreeWrapper() {
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}

struct TriangleWrapper{
    int v0, v1, v2;
};

void EmbreeWrapper::createMeshGeometry(const Mesh& mesh)
{
    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    glm::vec3* vertices = (glm::vec3*) rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX,
                            0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3), mesh.vertices.size());

    TriangleWrapper* indices = (TriangleWrapper*) rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX,
                            0, RTC_FORMAT_UINT3, sizeof(TriangleWrapper), mesh.indices.size()/3);


    for(unsigned i=0;i<mesh.vertices.size();i++){
        vertices[i] = mesh.vertices[i].position;
    }

    for(unsigned i=0;i<mesh.indices.size()/3;i++){
        indices[i].v0 = mesh.indices[3*i];
        indices[i].v1 = mesh.indices[3*i+1];
        indices[i].v2 = mesh.indices[3*i+2];
    }

    rtcCommitGeometry(geometry);
    rtcAttachGeometry(scene, geometry);
    rtcReleaseGeometry(geometry);
}


HitData EmbreeWrapper::cast(Ray r, float maxDistance) { 
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    RTCRayHit hit;
    hit.ray = r.toRTCRay(maxDistance);
    hit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect1(scene, &context, &hit);

    HitData ans;
    if (hit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
    {
        ans.triangle = meshes[hit.hit.geomID].getTriangle(hit.hit.primID);
        ans.baryPos = glm::vec2(hit.hit.u, hit.hit.v);
        ans.distance = hit.ray.tfar;
        ans.pos = r.origin + ans.distance * r.direction;
    }
    return ans;
}
