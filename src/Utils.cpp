#include "Utils.h"

#include <limits>

const float Utils::INF = std::numeric_limits<float>::max();
const array<Utils::Axis, 3> Utils::AXES { Utils::Axis::X, Utils::Axis::Y, Utils::Axis::Z};

Utils::AxisGetter Utils::axisGetter(Axis axis) {
    static auto xGetter = [](glm::vec3 v) { return v.x; };
    static auto yGetter = [](glm::vec3 v) { return v.y; };
    static auto zGetter = [](glm::vec3 v) { return v.z; };

    switch (axis) {
    case Axis::X:
        return xGetter;
    case Axis::Y:
        return yGetter;
    case Axis::Z:
        return zGetter;
    }
}

void Utils::setOnAxis(glm::vec3& vec, float value, Axis axis){
    switch (axis) {
    case Axis::X:
        vec.x = value;
        break;
    case Axis::Y:
        vec.y = value;
        break;
    case Axis::Z:
        vec.z = value;
        break;
    }
}

float Utils::findBest(const vector<glm::vec3>& poses, Axis axis, CmpType cmpType){
    float ans = cmpType == CmpType::MIN ? INF : -INF;
    auto getter = axisGetter(axis);
    function<bool(float,float)> comparator = less<float>();
    if(cmpType == CmpType::MAX){
        comparator = greater<float>();
    }

    for (glm::vec3 pos : poses) {
        ans = min(ans, getter(pos), comparator);
    }
    return ans;
}