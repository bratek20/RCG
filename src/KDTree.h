#ifndef KD_TREE_H
#define KD_TREE_H

#include "AccStruct.h"
#include "Utils.h"

#include <memory>
#include <functional>

struct KDNode;
using KDNodePtr = KDNode*;

struct KDNode {
    enum Type {
        X_SPLIT = Utils::X,
        Y_SPLIT = Utils::Y,
        Z_SPLIT = Utils::Z,
        LEAF = 3
    };

    struct PlaneData {
        KDNodePtr near = nullptr;
        KDNodePtr far = nullptr;
        float t;
        bool intersects;
    };

    Type type;
    float split;
    KDNodePtr left;
    KDNodePtr right;
    vector<TrianglePtr> triangles;

    static KDNodePtr create(Type type);

    CastData leafIntersect(Ray r, float tMin, float tMax);
    PlaneData planeIntersect(Ray r);
};


class KDTree : public AccStruct {
    Bounds bounds;
    KDNodePtr root;
    int stopDepth;
    int stopTrianglesNum;

    struct SplitData{
        float value;
        Utils::Axis axis;
        bool isLeaf;
    };

public:
    KDTree(const vector<TrianglePtr>& triangles);
    CastData cast(Ray r) override;

private:
    KDNodePtr make(int depth, const vector<TrianglePtr>& triangles, Bounds bounds);  
    SplitData chooseSplit(int depth, const vector<TrianglePtr>& triangles, Bounds bounds);

    bool shouldBeLeaf(int depth, const vector<TrianglePtr>& triangles);
    KDNodePtr makeLeaf(const vector<TrianglePtr>& triangles);  
    
    float spatialMedian(Utils::Axis axis, const vector<TrianglePtr>& triangles);
    float objectMedian(Utils::Axis axis, const vector<TrianglePtr>& triangles);

    static float findBest(const vector<TrianglePtr>& triangles, float startValue, Utils::Axis axis, function<float(float, float)> comparator); 
    static vector<TrianglePtr> splitBy(float value, const vector<TrianglePtr>& triangles, Utils::Axis axis, function<bool(float, float)> comparator);     

    CastData traverse(KDNodePtr node, Ray r, float tMin, float tMax);
};
#endif