#include "SAH.h"

vector<SAH::BoundEdge> SAH::edges;

void SAH::init(int maxTrianglesNum){
    edges.resize(maxTrianglesNum * 2);
}

SAH::SplitData SAH::bestSplit(const Bounds& bounds,
                              const vector<TrianglePtr> &triangles, const vector<int>& triIndices) {
    Utils::Axis firstAxis = bounds.maximumExtent();
    Utils::Axis axes[] = {firstAxis, Utils::next(firstAxis),
                          Utils::next(firstAxis, 2)};

    float bruteCost = INTERSECTION_COST * triIndices.size();
    SplitData ans;
    for (auto axis : axes) {
        SplitData data = bestSplitFor(axis, bounds, triangles, triIndices);
        if (data.cost < ans.cost) {
            ans = data;
            //break;
        }
    }

    ans.badRefine = bruteCost < ans.cost;
    return ans;
}

SAH::SplitData SAH::bestSplitFor(Utils::Axis axis, const Bounds& bounds,
                                 const vector<TrianglePtr> &triangles, const vector<int>& triIndices) {
    int trianglesNum = triIndices.size();
    for (int i=0;i<trianglesNum;i++) {
        Bounds b = triangles[triIndices[i]]->getBounds();
        edges[2 * i].init(b.pMin[axis], false);
        edges[2 * i + 1].init(b.pMax[axis], true);
    }

    sort(edges.begin(), edges.begin() + 2 * trianglesNum,
         [](const BoundEdge &e0, const BoundEdge &e1) -> bool {
             if (e0.t == e1.t)
                 return e0.end < e1.end;
             else
                 return e0.t < e1.t;
         });

    float totalSA = bounds.surfaceArea();
    float invTotalSA = 1 / totalSA;

    int nBelow = 0, nAbove = trianglesNum;
    SplitData ans;
    ans.axis = axis;
    for (int i = 0; i < 2 * trianglesNum; i++) {
        if (edges[i].end) {
            nAbove--;
        }

        float edgeT = edges[i].t;
        if (edgeT > bounds.pMin[axis] && edgeT < bounds.pMax[axis]) {
            auto splits = bounds.split(axis, edgeT);
            float belowSA = splits.first.surfaceArea();
            float aboveSA = splits.second.surfaceArea();

            float pBelow = belowSA * invTotalSA;
            float pAbove = aboveSA * invTotalSA;
            float eb = (nAbove == 0 || nBelow == 0) ? EMPTY_BONUS : 0;
            float cost =
                TRAVERSAL_COST +
                INTERSECTION_COST * (1 - eb) * (pBelow * nBelow + pAbove * nAbove);

            if(cost < ans.cost){
                ans.cost = cost;
                ans.value = edgeT;
            }
        }

        if (!edges[i].end) {
            nBelow++;
        }
    }

    return ans;
}