#include "KDTree.h"
#include "SAH.h"

#include <algorithm>

#define USE_SAH 1

KDNodePtr KDNode::create(Type type) {
    KDNodePtr node = new KDNode();
    node->type = type;
    return node;
}

CastData KDNode::leafIntersect(Ray r, float tMin, float tMax) {
    CastData ans;
    ans.distance = Utils::INF;

    for (auto tri : triangles) {
        CastData data = AccStruct::intersect(r, tri);
        if (data.intersects() && data.distance < ans.distance &&
            data.distance >= tMin && data.distance < tMax) {
            ans = data;
        }
    }
    return ans;
}

KDNode::PlaneData KDNode::planeIntersect(Ray r) {
    float o = r.origin[type];
    float d = r.direction[type];

    PlaneData ans;
    if (d != 0) {
        ans.intersects = true;
        ans.t = (split - o) / d;
        ans.near = d > 0 ? left : right;
        ans.far = d > 0 ? right : left;
    } else {
        ans.intersects = false;
        ans.near = o < split ? left : right;
    }
    return ans;
}

KDTree::KDTree(const vector<TrianglePtr> &triangles) : AccStruct(triangles) {
    stopDepth = 8 + 1.3 * log(triangles.size());
    stopTrianglesNum = 8;

    int trianglesNum = triangles.size(); 
    SAH::init(trianglesNum);
    vector<glm::vec3> poses(trianglesNum * 3);
    for(int i = 0;i<trianglesNum;i++){
        auto triPos = triangles[i]->getPositions();
        for(int j=0;j<3;j++){
            poses[3*i+j] = triPos[j];
        }
    }
    
    bounds = Bounds(poses);
    root = make(0, triangles, bounds);
}

KDNodePtr KDTree::make(int depth, const vector<TrianglePtr> &triangles,
                       Bounds bounds) {
    SplitData data = chooseSplit(depth, triangles, bounds);
    if(data.isLeaf){
        return makeLeaf(triangles);
    }

    Utils::Axis axis = data.axis;
    KDNode::Type type = static_cast<KDNode::Type>(axis);
    KDNodePtr node = KDNode::create(type);
    node->split = data.value; 

    auto newBounds = bounds.split(axis, data.value);
    auto left = splitBy(data.value, triangles, axis, less<float>());
    auto right = splitBy(data.value, triangles, axis, greater_equal<float>());

    node->left = make(depth + 1, left, newBounds.first);
    node->right = make(depth + 1, right, newBounds.second);
    return node;
}

KDTree::SplitData KDTree::chooseSplit(int depth, const vector<TrianglePtr>& triangles, Bounds bounds){
    SplitData ans;
#if USE_SAH == 1    
    SAH::SplitData data = SAH::bestSplit(bounds, triangles);
    ans.value = data.value;
    ans.axis = data.axis;
    ans.isLeaf = data.failed() || shouldBeLeaf(depth, triangles);
#else
    ans.axis = static_cast<Utils::Axis>(depth % 3);
    ans.value = spatialMedian(ans.axis, triangles);
    ans.isLeaf = shouldBeLeaf(depth, triangles);
#endif
    return ans;
}

CastData KDTree::cast(Ray r) { return traverse(root, r, 0, Utils::INF); }

bool KDTree::shouldBeLeaf(int depth, const vector<TrianglePtr> &triangles) {
    return depth == stopDepth || triangles.size() <= stopTrianglesNum;
}

KDNodePtr KDTree::makeLeaf(const vector<TrianglePtr> &triangles) {
    KDNodePtr node = KDNode::create(KDNode::LEAF);
    node->triangles = triangles;
    return node;
}

float KDTree::spatialMedian(Utils::Axis axis,
                            const vector<TrianglePtr> &triangles) {
    float minVal = findBest(triangles, Utils::INF, axis, less<float>());
    float maxVal = findBest(triangles, -Utils::INF, axis, greater<float>());
    return (minVal + maxVal) / 2;
}

float KDTree::objectMedian(Utils::Axis axis,
                           const vector<TrianglePtr> &triangles) {
    vector<float> centers;
    for (auto &tri : triangles) {
        float center = 0;
        for (glm::vec3 pos : tri->getPositions()) {
            center += pos[axis];
        }
        centers.push_back(center / 3);
    }

    size_t n = centers.size() / 2;
    nth_element(centers.begin(), centers.begin() + n, centers.end());
    return centers[n];
}

vector<TrianglePtr> KDTree::splitBy(float value,
                                    const vector<TrianglePtr> &triangles,
                                    Utils::Axis axis,
                                    function<bool(float, float)> comparator) {
    vector<TrianglePtr> ans;
    for (auto tri : triangles) {
        for (glm::vec3 pos : tri->getPositions()) {
            if (comparator(pos[axis], value)) {
                ans.push_back(tri);
                break;
            }
        }
    }
    return ans;
}

float KDTree::findBest(const vector<TrianglePtr> &triangles, float startValue,
                       Utils::Axis axis,
                       function<float(float, float)> comparator) {
    float ans = startValue;
    for (auto tri : triangles) {
        for (glm::vec3 pos : tri->getPositions()) {
            ans = min(ans, pos[axis], comparator);
        }
    }
    return ans;
}

CastData KDTree::traverse(KDNodePtr node, Ray r, float tMin, float tMax) {
    if (node == nullptr) {
        return CastData();
    }
    if (node->type == KDNode::LEAF) {
        return node->leafIntersect(r, tMin, tMax);
    }

    KDNode::PlaneData data = node->planeIntersect(r);
    if (!data.intersects || data.t >= tMax) {
        return traverse(data.near, r, tMin, tMax);
    }
    if (data.t <= tMin) {
        return traverse(data.far, r, tMin, tMax);
    }

    CastData ans = traverse(data.near, r, tMin, data.t);
    if (ans.intersects()) {
        return ans;
    }
    return traverse(data.far, r, data.t, tMax);
}