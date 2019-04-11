#ifndef SAH_H
#define SAH_H

#include "Utils.h"
#include "Mesh.h"

class SAH {
    static constexpr int TRAVERSAL_COST = 80;
    static constexpr int INTERSECTION_COST = 1;
    static constexpr float EMPTY_BONUS = 0.5f; 

    struct BoundEdge {
        float t;
        bool end;
        
        void init(float t, bool end){
            this->t = t;
            this->end = end;
        }
    };

    static vector<BoundEdge> edges;

public:
    struct SplitData{
        Utils::Axis axis;
        float value;
        float cost = Utils::INF;
        bool badRefine = false;

        bool failed() const {
            return cost == Utils::INF; 
        } 
    };

    static void init(int maxTrianglesNum);
    static SplitData bestSplit(Bounds bounds, const vector<TrianglePtr>& triangles, const vector<int>& triIndices);

private:
    static SplitData bestSplitFor(Utils::Axis axis, Bounds bounds, const vector<TrianglePtr>& triangles, const vector<int>& triIndices);
};

#endif