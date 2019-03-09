#include "Program3D.h"
#include "Light.h"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

const int Program3D::MAX_LIGHTS = 10;

Program3D::Program3D(std::string vsName, std::string fsName) : ShaderProgram(vsName, fsName) {
    MVPId = glGetUniformLocation(id, "MVP");
    viewMatId = glGetUniformLocation(id, "V");
	worldMatId = glGetUniformLocation(id, "M");
    meshColorId = glGetUniformLocation(id, "MeshColor");

    playerPositionId = glGetUniformLocation(id, "PlayerPosition_worldspace");
    lightsNumId = glGetUniformLocation(id, "LightsNum");
    lightPosId = glGetUniformLocation(id, "LightPosition_worldspace");
    lightPowerId = glGetUniformLocation(id, "LightPower");
    lightColorId = glGetUniformLocation(id, "LightColor");
    lightCoefficientsId = glGetUniformLocation(id, "LightDistanceCoefficients");

    textureId  = glGetUniformLocation(id, "TextureSampler");
}

void Program3D::setProjectionMat(const glm::mat4& mat){
    projectionMat = mat;
}

void Program3D::setViewMat(const glm::mat4& mat){
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &mat[0][0]);
    viewMat = mat;
}

void Program3D::addLight(LightPtr light){
    lights.push_back(light);
}

void Program3D::applyLights(){
    lights.erase(
        std::remove_if(lights.begin(), lights.end(), [](WeakLightPtr l){return l.expired();}),
        lights.end());

    int size = std::min(static_cast<int>(lights.size()), MAX_LIGHTS);
    glUniform1i(lightsNumId, size);

    glm::vec3 pos[size];
    float power[size];
    glm::vec3 color[size];
    glm::vec3 coefficient[size];
    for(int i=0;i<size;i++){
        auto light = lights[i].lock();
        pos[i] = light->getWorldPosition();
        power[i] = light->getPower();
        color[i] = static_cast<glm::vec3>(light->getColor());
        coefficient[i] = light->getCoefficients();
    }

    glUniform3fv(lightPosId, size, glm::value_ptr(pos[0]));
    glUniform1fv(lightPowerId, size, power);
    glUniform3fv(lightColorId, size, glm::value_ptr(color[0]));
    glUniform3fv(lightCoefficientsId, size, glm::value_ptr(coefficient[0]));
}

void Program3D::applyPlayerPosition(glm::vec3 worldPlayerPos){
    glUniform3f(playerPositionId, worldPlayerPos.x, worldPlayerPos.y, worldPlayerPos.z);
}

void Program3D::applyWorldMat(const glm::mat4& worldMat){
    glm::mat4 MVP = projectionMat * viewMat * worldMat;
    glUniformMatrix4fv(worldMatId, 1, GL_FALSE, &worldMat[0][0]);
    glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);
}

void Program3D::applyColor(const Color& c){
    glUniform3f(meshColorId, c.r, c.g, c.b);
}

void Program3D::applyTexture(GLuint texture){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureId, 0);
}
