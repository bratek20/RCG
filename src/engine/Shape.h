#ifndef SHAPE_H
#define SHAPE_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shape;
using ShapePtr = std::shared_ptr<Shape>;

class Shape{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint uvBuffer;

public:
    static ShapePtr create(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);
    static ShapePtr create(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals);
    ~Shape();

    void apply();

    unsigned verticesNum() const;
    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec3>& getNormals() const;
    const std::vector<glm::vec2>& getUvs() const;

private:
    Shape(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);
};

#endif