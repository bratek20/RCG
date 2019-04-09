#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <glm/glm.hpp>
#include <vector>

using namespace std;

class Utils {
public:
    enum Axis {
        X = 0,
        Y = 1,
        Z = 2
    };

    enum class CmpType {
        MIN,
        MAX
    };

    static const float INF;
    static const array<Axis, 3> AXES;
    static Axis next(Axis axis, int step = 1);

    static float findBest(const vector<glm::vec3>& poses, Axis axis, CmpType cmpType); 
};

#endif