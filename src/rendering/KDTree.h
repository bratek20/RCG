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
    std::vector<TrianglePtr> triangles;

    static KDNodePtr create(Type type);

    HitData leafIntersect(Ray r, float tMin, float tMax);
    PlaneData planeIntersect(Ray r);
};


class KDTree : public TrianglesAccStruct {
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
    KDTree(const std::vector<TrianglePtr>& triangles);
    HitData cast(Ray r, float maxDistance) override;

private:
    KDNodePtr make(int depth, const std::vector<TrianglePtr>& triangles, const Bounds& bounds);  
    SplitData chooseSplit(int depth, const std::vector<TrianglePtr>& triangles, const Bounds& bounds);

    bool shouldBeLeaf(int depth, const std::vector<TrianglePtr>& triangles);
    KDNodePtr makeLeaf(const std::vector<TrianglePtr>& triangles);  

    float objectMedian(Utils::Axis axis, const std::vector<TrianglePtr>& triangles);

    static float findBest(const std::vector<TrianglePtr>& triangles, float startValue, Utils::Axis axis, std::function<float(float, float)> comparator); 
    static std::vector<TrianglePtr> splitBy(float value, const std::vector<TrianglePtr>& triangles, Utils::Axis axis, std::function<bool(float, float)> comparator);     

    HitData traverse(KDNodePtr node, Ray r, float tMin, float tMax);
};
#endif