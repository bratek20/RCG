#include "Program3D.h"

Program3D::Program3D(std::string vsName, std::string fsName) : Shader(vsName, fsName) {
    MVPId = glGetUniformLocation(id, "MVP");
    viewMatId = glGetUniformLocation(id, "V");
	worldMatId = glGetUniformLocation(id, "M");
    meshColorId = glGetUniformLocation(id, "MeshColor");

    playerPositionId = glGetUniformLocation(id, "PlayerPosition_worldspace");

    textureId  = glGetUniformLocation(id, "TextureSampler");
}

void Program3D::setProjectionMat(const glm::mat4& mat){
    projectionMat = mat;
}

void Program3D::setViewMat(const glm::mat4& mat){
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &mat[0][0]);
    viewMat = mat;
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
