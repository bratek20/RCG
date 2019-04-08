#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <glm/glm.hpp>
#include <vector>

using namespace std;

class Utils {
public:
    enum class Axis {
        X,
        Y,
        Z
    };
     
    using AxisGetter = function<float(glm::vec3)>;

    enum class CmpType {
        MIN,
        MAX
    };

    static const float INF;
    static const array<Axis, 3> AXES;

    static AxisGetter axisGetter(Axis axis); 
    static void setOnAxis(glm::vec3& vec, float value, Axis axis);
    static float findBest(const vector<glm::vec3>& poses, Axis axis, CmpType cmpType); 
};

#endif