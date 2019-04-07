#include "KDTree.h"
#include "Globals.h"

#include <algorithm>

KDNodePtr KDNode::create(Type type) {
    KDNodePtr node = new KDNode();
    node->type = type;
    return node;
}

function<float(glm::vec3)> KDNode::getGetter(Type splitType) {
    static auto xGetter = [](glm::vec3 v) { return v.x; };
    static auto yGetter = [](glm::vec3 v) { return v.y; };
    static auto zGetter = [](glm::vec3 v) { return v.z; };

    switch (splitType) {
    case X_SPLIT:
        return xGetter;
    case Y_SPLIT:
        return yGetter;
    case Z_SPLIT:
        return zGetter;
    default:
        throw runtime_error("node type not supported");
    }
}

CastData KDNode::leafIntersect(Ray r, float tMin, float tMax){
    CastData ans;
    ans.distance = Globals::INF;

    for (auto tri : triangles) {
        CastData data = AccStruct::intersect(r, tri);
        if (data.intersects() && data.distance < ans.distance &&
            data.distance >= tMin && data.distance < tMax) {
            ans = data;
        }
    }
    return ans;
}

KDNode::PlaneData KDNode::planeIntersect(Ray r){
    auto getter = getGetter(type);
    float o = getter(r.origin);
    float d = getter(r.direction);
    
    PlaneData ans;
    if(d != 0){
        ans.intersects = true;
        ans.t = (split - o)/d;
        ans.near = d > 0 ? left : right;
        ans.far = d > 0 ? right : left;
    }
    else{
        ans.intersects = false;
        ans.near = o < split ? left : right;
    }
    return ans;
}

KDTree::KDTree(const vector<TrianglePtr> &triangles) : AccStruct(triangles) {
    stopDepth = 8 + 1.3 * log(triangles.size());
    stopTrianglesNum = 8;
    root = make(0, triangles);
}

KDNodePtr KDTree::make(int depth, const vector<TrianglePtr> &triangles) {
    KDNode::Type type = calcNodeType(depth, triangles);
    KDNodePtr node = KDNode::create(type);
    if (node->type == KDNode::LEAF) {
        node->triangles = triangles;
        return node;
    }

    SplitData splitData = splitTriangles(type, triangles);
    node->split = splitData.value;
    node->left = make(depth + 1, splitData.left);
    node->right = make(depth + 1, splitData.right);
    return node;
}

CastData KDTree::cast(Ray r) {
    return traverse(root, r, 0, Globals::INF);
}

KDNode::Type KDTree::calcNodeType(int depth,
                                  const vector<TrianglePtr> &triangles) {
    if (depth == stopDepth || triangles.size() <= stopTrianglesNum) {
        return KDNode::LEAF;
    }
    return static_cast<KDNode::Type>(depth % 3);
}

KDTree::SplitData KDTree::splitTriangles(KDNode::Type splitType,
                                         const vector<TrianglePtr> &triangles) {
    auto getter = KDNode::getGetter(splitType);
    float minVal = findBest(triangles, Globals::INF, getter,
                            less<float>());
    float maxVal = findBest(triangles, -Globals::INF, getter,
                            greater<float>());

    SplitData data;
    data.value = (minVal + maxVal) / 2;
    data.left = splitBy(data.value, triangles, getter, less<float>());
    data.right = splitBy(data.value, triangles, getter, greater_equal<float>());
    return data;
}

vector<TrianglePtr> KDTree::splitBy(float value,
                                    const vector<TrianglePtr> &triangles,
                                    function<float(glm::vec3)> getter,
                                    function<bool(float, float)> comparator) {
    vector<TrianglePtr> ans;
    for (auto tri : triangles) {
        for (glm::vec3 pos : tri->getPositions()) {
            if (comparator(getter(pos), value)) {
                ans.push_back(tri);
                break;
            }
        }
    }
    return ans;
}

float KDTree::findBest(const vector<TrianglePtr> &triangles, float startValue,
                       function<float(glm::vec3)> getter,
                       function<float(float, float)> comparator) {
    float ans = startValue;
    for (auto tri : triangles) {
        for (glm::vec3 pos : tri->getPositions()) {
            ans = min(ans, getter(pos), comparator);
        }
    }
    return ans;
}

CastData KDTree::traverse(KDNodePtr node, Ray r, float tMin, float tMax) {
    if(node == nullptr){
        return CastData();
    }
    if(node->type == KDNode::LEAF){
        return node->leafIntersect(r, tMin, tMax);
    }

    KDNode::PlaneData data = node->planeIntersect(r);
    if(!data.intersects || data.t >= tMax){
        return traverse(data.near, r, tMin, tMax);
    }
    if(data.t <= tMin){
        return traverse(data.far, r, tMin, tMax);    
    }

    CastData ans = traverse(data.near, r, tMin, data.t);
    if(ans.intersects()){
        return ans;
    }
    return traverse(data.far, r, data.t, tMax);

}