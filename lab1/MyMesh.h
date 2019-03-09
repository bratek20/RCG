#ifndef MY_MESH_H
#define MY_MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "Program3D.h"
#include "Shape.h"
#include "Color.h"
#include "Assets.h"

class MyMesh;
using MyMeshPtr = std::shared_ptr<MyMesh>;

class Light;
using LightPtr = std::shared_ptr<Light>;
using WeakLightPtr = std::weak_ptr<Light>;

class MyMesh {   
    static Program3D program;

    ShapePtr shape;
    Color color;
    GLuint texture;
    GLuint renderType;

public:
    static MyMeshPtr create(ShapePtr shape, Color color = Colors::WHITE, GLuint texture = Assets::WHITE_TEXTURE, GLuint renderType = GL_TRIANGLES);

    static void init(); 
    static void clear();

    static void setProjectionMat(const glm::mat4& mat);
    static void setViewMat(const glm::mat4& mat);
    
    static void addLight(LightPtr light);
    static void applyLights();
    static void applyPlayerPosition(glm::vec3 worldPlayerPos);

    void render(const glm::mat4& worldMat);
    const std::vector<glm::vec3>& getLocalCoords() const;
    std::vector<glm::vec3> getWorldCoords(const glm::mat4& worldMat) const;
    
    Color getColor() const;

private:
    MyMesh(){}

    static void applyCommonUniforms(const glm::mat4& worldMat);
};

#endif