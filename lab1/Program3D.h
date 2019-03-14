#ifndef COMMON_PROGRAM_H
#define COMMON_PROGRAM_H

#include "Shader.h"
#include "Color.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Program3D : public Shader {
    GLuint MVPId;
    GLuint viewMatId;
    GLuint worldMatId;
    GLuint meshColorId;

    GLuint playerPositionId;
    GLuint textureId;

    glm::mat4 projectionMat;
    glm::mat4 viewMat;

public:
    static const int MAX_LIGHTS;

    Program3D(){}
    Program3D(std::string vsName, std::string fsName);

    void setProjectionMat(const glm::mat4& mat);
    void setViewMat(const glm::mat4& mat);
    void applyPlayerPosition(glm::vec3 worldPlayerPos);
    void applyWorldMat(const glm::mat4& worldMat);
    void applyColor(const Color& c); 
    void applyTexture(GLuint texture);
};

#endif