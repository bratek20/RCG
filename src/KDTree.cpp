#include "KDTree.h"

#include <algorithm>

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

CastData KDTree::cast(Ray r){
    CastData ans;
    ans.distance = numeric_limits<float>::max();
    
    for (auto tri : triangles) {
        CastData data = intersect(r, tri);
        if (data.intersects() && data.distance < ans.distance && data.distance > 0) {
            ans = data;
        }
    }
    return ans;
}

KDNode::Type KDTree::calcNodeType(int depth, const vector<TrianglePtr>& triangles){
    if(depth == stopDepth || triangles.size() <= stopTrianglesNum){
        return KDNode::LEAF; 
    }
    return static_cast<KDNode::Type>(depth % 3);
}

KDTree::SplitData KDTree::splitTriangles(KDNode::Type splitType, const vector<TrianglePtr>& triangles){
    auto getter = getGetter(splitType);
    float minVal = findBest(triangles, numeric_limits<float>::max(), getter, less<float>());
    float maxVal = findBest(triangles, -numeric_limits<float>::max(), getter, greater<float>());

    SplitData data;
    data.value = (minVal + maxVal)/2;
    data.left = splitBy(data.value, triangles, getter, less_equal<float>());
    data.right = splitBy(data.value, triangles, getter, greater_equal<float>());
    return data;
}

vector<TrianglePtr> KDTree::splitBy(float value, const vector<TrianglePtr>& triangles, function<float(glm::vec3)> getter, function<bool(float, float)> comparator){
    vector<TrianglePtr> ans;
    for(auto tri : triangles){
        for(glm::vec3 pos : tri->getPositions()){
            if(comparator(getter(pos), value)){
                ans.push_back(tri);
                break;
            }
        }
    }
    return ans;
}

float KDTree::findBest(const vector<TrianglePtr>& triangles, float startValue, function<float(glm::vec3)> getter, function<float(float, float)> comparator){
    float ans = startValue;
    for(auto tri : triangles){
        for(glm::vec3 pos : tri->getPositions()){
            ans = min(ans, getter(pos), comparator);
        }
    }
    return ans;
}

function<float(glm::vec3)> KDTree::getGetter(KDNode::Type splitType){
    static auto xGetter = [](glm::vec3 v){return v.x;};
    static auto yGetter = [](glm::vec3 v){return v.y;};
    static auto zGetter = [](glm::vec3 v){return v.z;};

    switch(splitType){
        case KDNode::X_SPLIT:
            return xGetter;
        case KDNode::Y_SPLIT:
            return yGetter;
        case KDNode::Z_SPLIT:
            return zGetter;
        default:
            throw runtime_error("node type not supported");
    }
}