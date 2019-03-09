#ifndef COMMON_PROGRAM_H
#define COMMON_PROGRAM_H

#include "ShaderProgram.h"
#include "Color.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Light;
using LightPtr = std::shared_ptr<Light>;
using WeakLightPtr = std::weak_ptr<Light>;

class Program3D : public ShaderProgram {
    GLuint MVPId;
    GLuint viewMatId;
    GLuint worldMatId;
    GLuint meshColorId;

    GLuint playerPositionId;
    GLuint lightsNumId;
    GLuint lightPosId;
    GLuint lightPowerId;
    GLuint lightColorId;
    GLuint lightCoefficientsId;

    GLuint textureId;

    glm::mat4 projectionMat;
    glm::mat4 viewMat;
    std::vector<WeakLightPtr> lights;

public:
    static const int MAX_LIGHTS;

    Program3D(){}
    Program3D(std::string vsName, std::string fsName);

    void setProjectionMat(const glm::mat4& mat);
    void setViewMat(const glm::mat4& mat);
    void addLight(LightPtr light);
    void applyLights();
    void applyPlayerPosition(glm::vec3 worldPlayerPos);
    void applyWorldMat(const glm::mat4& worldMat);
    void applyColor(const Color& c); 
    void applyTexture(GLuint texture);
};

#endif