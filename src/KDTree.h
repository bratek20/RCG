#ifndef KD_TREE_H
#define KD_TREE_H

#include "AccStruct.h"
#include <memory>
#include <functional>

struct KDNode;
using KDNodePtr = KDNode*;

struct KDNode {
    enum Type {
        X_SPLIT = 0,
        Y_SPLIT = 1,
        Z_SPLIT = 2,
        LEAF = 3
    };

    struct PlaneData {
        float t;
        KDNodePtr near = nullptr;
        KDNodePtr far = nullptr;
        bool intersetcs(){
            return near != nullptr || far != nullptr;
        }
    };

    Type type;
    float split;
    KDNodePtr left;
    KDNodePtr right;
    vector<TrianglePtr> triangles;

    static KDNodePtr create(Type type);
    static function<float(glm::vec3)> getGetter(Type splitType);

    CastData leafIntersect(Ray r, float tMin, float tMax);
    PlaneData planeIntersect(Ray r);
};


class KDTree : public AccStruct {
    struct SplitData {
        using Triangles = vector<TrianglePtr>;
        
        float value;
        Triangles left;
        Triangles right;
    };

    KDNodePtr root;
    int stopDepth;
    int stopTrianglesNum;

public:
    KDTree(const vector<TrianglePtr>& triangles);
    CastData cast(Ray r) override;

private:
    KDNodePtr make(int depth, const vector<TrianglePtr>& triangles);  
    KDNode::Type calcNodeType(int depth, const vector<TrianglePtr>& triangles);  
    SplitData splitTriangles(KDNode::Type splitType, const vector<TrianglePtr>& triangles);

    static float findBest(const vector<TrianglePtr>& triangles, float startValue, function<float(glm::vec3)> getter, function<float(float, float)> comparator); 
    static vector<TrianglePtr> splitBy(float value, const vector<TrianglePtr>& triangles, function<float(glm::vec3)> getter, function<bool(float, float)> comparator);     

    CastData traverse(KDNodePtr node, Ray r, float tMin, float tMax);
};
#endif