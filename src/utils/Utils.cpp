#include "Utils.h"

#include <limits>

using namespace std;

const float Utils::INF = std::numeric_limits<float>::max();
const array<Utils::Axis, 3> Utils::AXES { Utils::Axis::X, Utils::Axis::Y, Utils::Axis::Z};

Utils::Axis Utils::next(Axis axis, int step){
    return static_cast<Axis>((axis + step) % 3);
}

float Utils::findBest(const vector<glm::vec3>& poses, Axis axis, CmpType cmpType){
    float ans = cmpType == CmpType::MIN ? INF : -INF;
    function<bool(float,float)> comparator = less<float>();
    if(cmpType == CmpType::MAX){
        comparator = greater<float>();
    }

    for (glm::vec3 pos : poses) {
        ans = min(ans, pos[axis], comparator);
    }
    return ans;
}

glm::vec3 Utils::toVec(const aiVector3D& vec){
    return glm::vec3(vec.x, vec.y, vec.z);
}

Color Utils::toColor(const aiColor3D& col){
    return Color(col.r, col.g, col.b);
}

glm::vec2 Utils::toVec2(const glm::vec3& v, Axis toOmit){
    glm::vec2 ans;
    int i=0;
    for(Axis a : AXES){
        if(a != toOmit){
            ans[i++] = v[a];
        }
    }
    return ans;
}

ostream& operator<<(ostream& out, const glm::vec3& v){
    out << "{" <<v.x << "," << v.y << "," << v.z << "}";
    return out;
}