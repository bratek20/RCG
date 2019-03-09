#include "MyMesh.h"
#include "Light.h"
#include "Assets.h"

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

Program3D MyMesh::program;

MyMeshPtr MyMesh::create(ShapePtr shape, Color color, GLuint texture, GLuint renderType){
    auto mesh = MyMeshPtr(new MyMesh());
    mesh->shape = shape;
    mesh->color = color;
    mesh->texture = texture;
    mesh->renderType = renderType;
    return mesh;
}

void MyMesh::init(){
    glEnable(GL_DEPTH_TEST);

    program = Program3D("Program3D.vs", "Program3D.fs");
    setProjectionMat(glm::mat4(1.0f));
    setViewMat(glm::mat4(1.0f));
}

void MyMesh::clear(){
	program.clear();
}

void MyMesh::setProjectionMat(const glm::mat4& mat){
    program.setProjectionMat(mat);
}

void MyMesh::setViewMat(const glm::mat4& mat){
    program.setViewMat(mat);
}

void MyMesh::addLight(LightPtr light){
    program.addLight(light);
}

void MyMesh::applyLights(){
    program.applyLights();
}

void MyMesh::applyPlayerPosition(glm::vec3 worldPlayerPos){
    program.applyPlayerPosition(worldPlayerPos);
}

void MyMesh::render(const glm::mat4& worldMat){
    applyCommonUniforms(worldMat);
    program.applyColor(color);
    program.applyTexture(texture);

    shape->apply();
    glDrawArrays(renderType, 0, shape->verticesNum());
}

void MyMesh::applyCommonUniforms(const glm::mat4& worldMat){
    program.applyWorldMat(worldMat);
}

const std::vector<glm::vec3>& MyMesh::getLocalCoords() const{
    return shape->getVertices();
}

std::vector<glm::vec3> MyMesh::getWorldCoords(const glm::mat4& worldMat) const{
    std::vector<glm::vec3> globalCoords;
    for(auto& localCoord : getLocalCoords()){
        globalCoords.push_back(static_cast<glm::vec3>(worldMat * glm::vec4(localCoord, 1.0f)));
    }
    return globalCoords;
}

Color MyMesh::getColor() const{
    return color;
}
