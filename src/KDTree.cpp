#include "KDTree.h"
#include "SAH.h"

#include <algorithm>

#define USE_SAH 1

KDNode::Type KDNode::getType(){
    return static_cast<KDNode::Type>(type & 3);
}

int KDNode::getTrianglesNum(){
    return trianglesNum >> 2;
}

int KDNode::getSecondChildIdx(){
    return secondChildIdx >> 2;
}

void KDNode::initLeaf(int indicesOffset, int trianglesNum){
    this->indicesOffset = indicesOffset;
    this->trianglesNum = (trianglesNum << 2) | LEAF;
}

void KDNode::initInterior(Utils::Axis axis, float split, int secondChildIdx){
    this->split = split;
    this->secondChildIdx = (secondChildIdx << 2) | axis;
}

CastData KDNode::leafIntersect(Ray r, float tMin, float tMax, const vector<TrianglePtr>& triangles, const vector<int>& triangleIndices) {
    CastData ans;
    ans.distance = Utils::INF;

    for (int i = 0; i < getTrianglesNum(); i++) {
        TrianglePtr tri = triangles[triangleIndices[indicesOffset + i]];
        CastData data = AccStruct::intersect(r, tri);
        if (data.intersects() && data.distance < ans.distance &&
            data.distance >= tMin && data.distance < tMax) {
            ans = data;
        }
    }
    return ans;
}

KDNode::PlaneData KDNode::planeIntersect(Ray r, int myIdx) {
    float o = r.origin[getType()];
    float d = r.direction[getType()];
    int left = myIdx+1;
    int right = getSecondChildIdx();
    
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
    
    vector<int> triIndices(trianglesNum);
    for(int i=0;i<trianglesNum;i++){
        triIndices[i] = i;
    }
    bounds = Bounds(poses);
    make(0, triIndices, bounds);
}

void KDTree::make(int depth, vector<int>& triIndices,
                       Bounds bounds) {
    int nodeIdx = nodes.size();
    nodes.push_back(KDNode());

    SplitData data = chooseSplit(depth, triIndices, bounds);
    if(data.isLeaf){
        nodes[nodeIdx].initLeaf(triangleIndices.size(), triIndices.size());
        Utils::addRange(triangleIndices, triIndices);
        return;
    }

    Utils::Axis axis = data.axis;
    auto newBounds = bounds.split(axis, data.value);
    auto left = splitBy(data.value, triIndices, axis, less<float>());
    auto right = splitBy(data.value, triIndices, axis, greater_equal<float>());

    make(depth + 1, left, newBounds.first);
    nodes[nodeIdx].initInterior(axis, data.value, nodes.size());
    make(depth + 1, right, newBounds.second);
}

KDTree::SplitData KDTree::chooseSplit(int depth, const vector<int>& triIndices, Bounds bounds){
    SplitData ans;
#if USE_SAH == 1    
    SAH::SplitData data = SAH::bestSplit(bounds, triangles, triIndices);
    ans.value = data.value;
    ans.axis = data.axis;
    ans.isLeaf = data.failed() || shouldBeLeaf(depth, triIndices);
#else
    ans.axis = static_cast<Utils::Axis>(depth % 3);
    ans.value = spatialMedian(ans.axis, triangles);
    ans.isLeaf = shouldBeLeaf(depth, triangles);
#endif
    return ans;
}

CastData KDTree::cast(Ray r, float maxDistance) { return traverse(0, r, 0, maxDistance); }

bool KDTree::shouldBeLeaf(int depth, const vector<int>& triIndices) {
    return depth == stopDepth || triIndices.size() <= stopTrianglesNum;
}

float KDTree::spatialMedian(Utils::Axis axis,
                            const vector<int>& triIndices) {
    float minVal = findBest(triIndices, Utils::INF, axis, less<float>());
    float maxVal = findBest(triIndices, -Utils::INF, axis, greater<float>());
    return (minVal + maxVal) / 2;
}

float KDTree::objectMedian(Utils::Axis axis,
                           const vector<int>& triIndices) {
    vector<float> centers;
    for (int idx : triIndices) {
        float center = 0;
        for (glm::vec3 pos : triangles[idx]->getPositions()) {
            center += pos[axis];
        }
        centers.push_back(center / 3);
    }

    size_t n = centers.size() / 2;
    nth_element(centers.begin(), centers.begin() + n, centers.end());
    return centers[n];
}

vector<int> KDTree::splitBy(float value,
                                    const vector<int>& triIndices,
                                    Utils::Axis axis,
                                    function<bool(float, float)> comparator) {
    vector<int> ans;
    for (int idx : triIndices) {
        for (glm::vec3 pos : triangles[idx]->getPositions()) {
            if (comparator(pos[axis], value)) {
                ans.push_back(idx);
                break;
            }
        }
    }
    return ans;
}

float KDTree::findBest(const vector<int>& triIndices, float startValue,
                       Utils::Axis axis,
                       function<float(float, float)> comparator) {
    float ans = startValue;
    for (int idx : triIndices) {
        for (glm::vec3 pos : triangles[idx]->getPositions()) {
            ans = min(ans, pos[axis], comparator);
        }
    }
    return ans;
}

CastData KDTree::traverse(int nodeIdx, Ray r, float tMin, float tMax) {
    KDNode& node = nodes[nodeIdx];
    if (node.getType() == KDNode::LEAF) {
        return node.leafIntersect(r, tMin, tMax, triangles, triangleIndices);
    }

    KDNode::PlaneData data = node.planeIntersect(r, nodeIdx);
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