#include "KDTree.h"

KDNodePtr KDNode::create(Type type){
    KDNodePtr node = new KDNode();
    node->type = type;
    return node;
}

KDTree::KDTree(const vector<TrianglePtr>& triangles) : AccStruct(triangles) {
    stopDepth = 8 + 1.3 * log(triangles.size());
    stopTrianglesNum = 8;
    root = make(0, triangles);
}

KDNodePtr KDTree::make(int depth, const vector<TrianglePtr>& triangles){
    KDNode::Type type = calcNodeType(depth, triangles);
    KDNodePtr node = KDNode::create(type);
    if(type == KDNode::LEAF)
    {
        node->triangles = triangles;
        return node;
    }

    SplitData splitData = splitTriangles(type, triangles);
    node->split = splitData.value; 
    node->left = make(depth + 1, splitData.left);
    node->right = make(depth + 1, splitData.right);
    return node;
} 

KDNode::Type KDTree::calcNodeType(int depth, const vector<TrianglePtr>& triangles){
    if(depth == stopDepth || triangles.size() <= stopTrianglesNum){
        return KDNode::LEAF; 
    }
    return static_cast<KDNode::Type>(depth % 3);
}

KDTree::SplitData KDTree::splitTriangles(KDNode::Type splitType, const vector<TrianglePtr>& triangles){
    
}