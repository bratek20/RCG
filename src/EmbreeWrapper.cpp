#include "EmbreeWrapper.h"

using namespace std;

EmbreeWrapper::EmbreeWrapper(const vector<Mesh>& meshes) : meshes(meshes) {
    device = rtcNewDevice(NULL);
    scene = rtcNewScene(device);
    geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);


    rtcCommitGeometry(geometry);    
    rtcAttachGeometry(scene, geometry);
    rtcCommitScene(scene);
}

EmbreeWrapper::~EmbreeWrapper() {
    rtcReleaseGeometry(geometry);
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}

// unsigned int addCube (RTCScene scene_i)
// {
//   /* create a triangulated cube with 12 triangles and 8 vertices */
//   RTCGeometry mesh = rtcNewGeometry(g_device, RTC_GEOMETRY_TYPE_TRIANGLE);

//   /* create face and vertex color arrays */
//   face_colors = (Vec3fa*) alignedMalloc(12*sizeof(Vec3fa),16);
//   vertex_colors = (Vec3fa*) alignedMalloc(8*sizeof(Vec3fa),16);

//   /* set vertices and vertex colors */
//   Vertex* vertices = (Vertex*) rtcSetNewGeometryBuffer(mesh,RTC_BUFFER_TYPE_VERTEX,0,RTC_FORMAT_FLOAT3,sizeof(Vertex),8);
//   vertex_colors[0] = Vec3fa(0,0,0); vertices[0].x = -1; vertices[0].y = -1; vertices[0].z = -1;
//   vertex_colors[1] = Vec3fa(0,0,1); vertices[1].x = -1; vertices[1].y = -1; vertices[1].z = +1;
//   vertex_colors[2] = Vec3fa(0,1,0); vertices[2].x = -1; vertices[2].y = +1; vertices[2].z = -1;
//   vertex_colors[3] = Vec3fa(0,1,1); vertices[3].x = -1; vertices[3].y = +1; vertices[3].z = +1;
//   vertex_colors[4] = Vec3fa(1,0,0); vertices[4].x = +1; vertices[4].y = -1; vertices[4].z = -1;
//   vertex_colors[5] = Vec3fa(1,0,1); vertices[5].x = +1; vertices[5].y = -1; vertices[5].z = +1;
//   vertex_colors[6] = Vec3fa(1,1,0); vertices[6].x = +1; vertices[6].y = +1; vertices[6].z = -1;
//   vertex_colors[7] = Vec3fa(1,1,1); vertices[7].x = +1; vertices[7].y = +1; vertices[7].z = +1;

//   /* set triangles and face colors */
//   int tri = 0;
//   Triangle* triangles = (Triangle*) rtcSetNewGeometryBuffer(mesh,RTC_BUFFER_TYPE_INDEX,0,RTC_FORMAT_UINT3,sizeof(Triangle),12);

//   // left side
//   face_colors[tri] = Vec3fa(1,0,0); triangles[tri].v0 = 0; triangles[tri].v1 = 1; triangles[tri].v2 = 2; tri++;
//   face_colors[tri] = Vec3fa(1,0,0); triangles[tri].v0 = 1; triangles[tri].v1 = 3; triangles[tri].v2 = 2; tri++;

//   // right side
//   face_colors[tri] = Vec3fa(0,1,0); triangles[tri].v0 = 4; triangles[tri].v1 = 6; triangles[tri].v2 = 5; tri++;
//   face_colors[tri] = Vec3fa(0,1,0); triangles[tri].v0 = 5; triangles[tri].v1 = 6; triangles[tri].v2 = 7; tri++;

//   // bottom side
//   face_colors[tri] = Vec3fa(0.5f);  triangles[tri].v0 = 0; triangles[tri].v1 = 4; triangles[tri].v2 = 1; tri++;
//   face_colors[tri] = Vec3fa(0.5f);  triangles[tri].v0 = 1; triangles[tri].v1 = 4; triangles[tri].v2 = 5; tri++;

//   // top side
//   face_colors[tri] = Vec3fa(1.0f);  triangles[tri].v0 = 2; triangles[tri].v1 = 3; triangles[tri].v2 = 6; tri++;
//   face_colors[tri] = Vec3fa(1.0f);  triangles[tri].v0 = 3; triangles[tri].v1 = 7; triangles[tri].v2 = 6; tri++;

//   // front side
//   face_colors[tri] = Vec3fa(0,0,1); triangles[tri].v0 = 0; triangles[tri].v1 = 2; triangles[tri].v2 = 4; tri++;
//   face_colors[tri] = Vec3fa(0,0,1); triangles[tri].v0 = 2; triangles[tri].v1 = 6; triangles[tri].v2 = 4; tri++;

//   // back side
//   face_colors[tri] = Vec3fa(1,1,0); triangles[tri].v0 = 1; triangles[tri].v1 = 5; triangles[tri].v2 = 3; tri++;
//   face_colors[tri] = Vec3fa(1,1,0); triangles[tri].v0 = 3; triangles[tri].v1 = 5; triangles[tri].v2 = 7; tri++;

//   rtcSetGeometryVertexAttributeCount(mesh,1);
//   rtcSetSharedGeometryBuffer(mesh,RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE,0,RTC_FORMAT_FLOAT3,vertex_colors,0,sizeof(Vec3fa),8);
  
//   rtcCommitGeometry(mesh);
//   unsigned int geomID = rtcAttachGeometry(scene_i,mesh);
//   rtcReleaseGeometry(mesh);
//   return geomID;
// }


HitData EmbreeWrapper::cast(Ray r, float maxDistance) { 
   
}

/* task that renders a single screen tile */
// Vec3fa renderPixelStandard(float x, float y, const ISPCCamera& camera, RayStats& stats)
// {
//   RTCIntersectContext context;
//   rtcInitIntersectContext(&context);
  
//   /* initialize ray */
//   Ray ray(Vec3fa(camera.xfm.p), Vec3fa(normalize(x*camera.xfm.l.vx + y*camera.xfm.l.vy + camera.xfm.l.vz)), 0.0f, inf);

//   /* intersect ray with scene */
//   rtcIntersect1(g_scene,&context,RTCRayHit_(ray));
//   RayStats_addRay(stats);

//   /* shade pixels */
//   Vec3fa color = Vec3fa(0.0f);
//   if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
//   {
//     Vec3fa diffuse = face_colors[ray.primID];
//     color = color + diffuse*0.5f;
//     Vec3fa lightDir = normalize(Vec3fa(-1,-1,-1));

//     /* initialize shadow ray */
//     Ray shadow(ray.org + ray.tfar*ray.dir, neg(lightDir), 0.001f, inf, 0.0f);

//     /* trace shadow ray */
//     rtcOccluded1(g_scene,&context,RTCRay_(shadow));
//     RayStats_addShadowRay(stats);

//     /* add light contribution */
//     if (shadow.tfar >= 0.0f)
//       color = color + diffuse*clamp(-dot(lightDir,normalize(ray.Ng)),0.0f,1.0f);
//   }
//   return color;
// }
