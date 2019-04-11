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
        int near;
        int far;
        float t;
        bool intersects;
    };

    union { 
        int indicesOffset; // Leaf
        float split; // Interior 
    }; 
    
    union { 
        Type type; // Both
        int trianglesNum; // Leaf 
        int secondChildIdx; // Interior 
    };

    void initLeaf(int indicesOffset, int trianglesNum);
    void initInterior(Utils::Axis axis, float split, int secondChildIdx);

    CastData leafIntersect(Ray r, float tMin, float tMax, const vector<TrianglePtr>& triangles, const vector<int>& triangleIndices);
    PlaneData planeIntersect(Ray r, int myIdx);

    Type getType();
    int getTrianglesNum();
    int getSecondChildIdx();
};


class KDTree : public AccStruct {
    Bounds bounds;
    int stopDepth;
    int stopTrianglesNum;

    vector<KDNode> nodes;
    vector<int> triangleIndices;

    struct SplitData{
        float value;
        Utils::Axis axis;
        bool isLeaf;
    };

public:
    KDTree(const vector<TrianglePtr>& triangles);
    CastData cast(Ray r, float maxDistance) override;

private:
    void make(int depth, const vector<int>& triIndices, Bounds bounds);  
    SplitData chooseSplit(int depth, const vector<int>& triIndices, Bounds bounds);

    bool shouldBeLeaf(int depth, const vector<int>& triIndices); 
    
    float spatialMedian(Utils::Axis axis, const vector<int>& triIndices);
    float objectMedian(Utils::Axis axis, const vector<int>& triIndices);

    float findBest(const vector<int>& triIndices, float startValue, Utils::Axis axis, function<float(float, float)> comparator); 
    vector<int> splitBy(float value, const vector<int>& triIndices, Utils::Axis axis, function<bool(float, float)> comparator);     

    CastData traverse(int nodeIdx, Ray r, float tMin, float tMax);
};
#endif