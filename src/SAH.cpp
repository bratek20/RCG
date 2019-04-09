#include "SAH.h"

vector<SAH::BoundEdge> SAH::edges;

void SAH::init(int maxTrianglesNum){
    edges.resize(maxTrianglesNum * 2);
}

SAH::SplitData SAH::bestSplit(Bounds bounds,
                              const vector<TrianglePtr> &triangles) {
    Utils::Axis firstAxis = bounds.maximumExtent();
    Utils::Axis axes[] = {firstAxis, Utils::next(firstAxis),
                          Utils::next(firstAxis, 2)};

    float bruteCost = INTERSECTION_COST * triangles.size();
    SplitData ans;
    for (auto axis : axes) {
        SplitData data = bestSplitFor(axis, bounds, triangles);
        if (data.cost < ans.cost) {
            ans = data;
            break;
        }
    }

    ans.badRefine = bruteCost < ans.cost;
    return ans;
}

SAH::SplitData SAH::bestSplitFor(Utils::Axis axis, Bounds bounds,
                                 const vector<TrianglePtr> &triangles) {
    int trianglesNum = triangles.size();
    for (int i=0;i<trianglesNum;i++) {
        Bounds b = triangles[i]->getBounds();
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

    glm::vec3 d = bounds.diagonal();
    float totalSA = bounds.surfaceArea();
    float invTotalSA = 1 / totalSA;

    int nBelow = 0, nAbove = trianglesNum;
    SplitData ans;
    ans.axis = axis;
    for (unsigned i = 0; i < 2 * trianglesNum; i++) {
        if (edges[i].end) {
            nAbove--;
        }

        float edgeT = edges[i].t;
        if (edgeT > bounds.pMin[axis] && edgeT < bounds.pMax[axis]) {
            Utils::Axis otherAxis0 = Utils::next(axis), otherAxis1 = Utils::next(axis, 2);
            float belowSA = 2 * (d[otherAxis0] * d[otherAxis1] +
                                 (edgeT - bounds.pMin[axis]) *
                                     (d[otherAxis0] + d[otherAxis1]));
            float aboveSA = 2 * (d[otherAxis0] * d[otherAxis1] +
                                 (bounds.pMax[axis] - edgeT) *
                                     (d[otherAxis0] + d[otherAxis1]));

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