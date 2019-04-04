#ifndef KD_TREE_H
#define KD_TREE_H

#include "AccStruct.h"
#include <memory>

struct KDNode;
using KDNodePtr = KDNode*;

struct KDNode {
    enum Type {
        X_SPLIT = 0,
        Y_SPLIT = 1,
        Z_SPLIT = 2,
        LEAF = 3
    };

    Type type;
    float split;
    KDNodePtr left;
    KDNodePtr right;
    vector<TrianglePtr> triangles;

    static KDNodePtr create(Type type);
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

private:
    KDNodePtr make(int depth, const vector<TrianglePtr>& triangles);  
    KDNode::Type calcNodeType(int depth, const vector<TrianglePtr>& triangles);  
    SplitData splitTriangles(KDNode::Type splitType, const vector<TrianglePtr>& triangles); 
};
#endif