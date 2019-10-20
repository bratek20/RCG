#ifndef UTILS_H
#define UTILS_H

#include "Color.h"

#include <functional>
#include <glm/glm.hpp>
#include <vector>
#include <assimp/scene.h> 

std::ostream& operator<<(std::ostream& out, const glm::vec3& v);

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
    static const std::array<Axis, 3> AXES;
    static Axis next(Axis axis, int step = 1);

    static float findBest(const std::vector<glm::vec3>& poses, Axis axis, CmpType cmpType); 

    static glm::vec3 toVec(const aiVector3D& vec);
    static Color toColor(const aiColor3D& col);
    static glm::vec2 toVec2(const glm::vec3& v, Axis toOmit);
};

#endif