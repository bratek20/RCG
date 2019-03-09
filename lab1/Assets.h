#ifndef ASSETS_H
#define ASSETS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>
#include <functional>

#include "Shape.h"

class Assets {
    static GLuint vertexArrayIdx;
    static std::vector<GLfloat> cubeVertexData;
    static std::vector<GLfloat> cubeVertexNormalsData;

public:
    static ShapePtr CUBE;
    static ShapePtr SPHERE;
    static ShapePtr OBJ;
    static GLuint WHITE_TEXTURE;
    static GLuint TEXTURE;
    static GLuint GLASS_TEXTURE;
    static GLuint NOISE_TEXTURE;

    static void init(); 
    static void clear();

private:
    static ShapePtr loadShapeFromObj(std::string path); 
    static std::vector<glm::vec3> toVec3(const std::vector<float>& data);
    static void normalize(std::vector<glm::vec3>& data);
    static float maxDist(const std::vector<glm::vec3>& data, std::function<float(glm::vec3)> valueGetter);
};

#endif