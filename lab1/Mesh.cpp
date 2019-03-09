#include "Mesh.h"
#include "Light.h"
#include "Assets.h"

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

Program3D Mesh::program;

MeshPtr Mesh::create(ShapePtr shape, Color color, GLuint texture, GLuint renderType){
    auto mesh = MeshPtr(new Mesh());
    mesh->shape = shape;
    mesh->color = color;
    mesh->texture = texture;
    mesh->renderType = renderType;
    return mesh;
}

void Mesh::init(){
    glEnable(GL_DEPTH_TEST);

    program = Program3D("Program3D.vs", "Program3D.fs");
    setProjectionMat(glm::mat4(1.0f));
    setViewMat(glm::mat4(1.0f));
}

void Mesh::clear(){
	program.clear();
}

void Mesh::setProjectionMat(const glm::mat4& mat){
    program.setProjectionMat(mat);
}

void Mesh::setViewMat(const glm::mat4& mat){
    program.setViewMat(mat);
}

void Mesh::addLight(LightPtr light){
    program.addLight(light);
}

void Mesh::applyLights(){
    program.applyLights();
}

void Mesh::applyPlayerPosition(glm::vec3 worldPlayerPos){
    program.applyPlayerPosition(worldPlayerPos);
}

void Mesh::render(const glm::mat4& worldMat){
    applyCommonUniforms(worldMat);
    program.applyColor(color);
    program.applyTexture(texture);

    shape->apply();
    glDrawArrays(renderType, 0, shape->verticesNum());
}

void Mesh::applyCommonUniforms(const glm::mat4& worldMat){
    program.applyWorldMat(worldMat);
}

const std::vector<glm::vec3>& Mesh::getLocalCoords() const{
    return shape->getVertices();
}

std::vector<glm::vec3> Mesh::getWorldCoords(const glm::mat4& worldMat) const{
    std::vector<glm::vec3> globalCoords;
    for(auto& localCoord : getLocalCoords()){
        globalCoords.push_back(static_cast<glm::vec3>(worldMat * glm::vec4(localCoord, 1.0f)));
    }
    return globalCoords;
}

Color Mesh::getColor() const{
    return color;
}
