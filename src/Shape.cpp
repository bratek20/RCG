#include "Shape.h"

Shape::Shape(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs)
    : vertices(vertices), normals(normals), uvs(uvs) {
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
}

Shape::~Shape(){
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &vertexBuffer);
}

ShapePtr Shape::create(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs){
    return ShapePtr(new Shape(vertices, normals, uvs));
}

ShapePtr Shape::create(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals){
    std::vector<glm::vec2> uvs;
    return create(vertices, normals, uvs);
}

void Shape::apply(){
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    if(uvs.empty()){
        glDisableVertexAttribArray(2);
    }
    else{
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
    }   
}

unsigned Shape::verticesNum() const {
    return vertices.size() * 3;
}

const std::vector<glm::vec3>& Shape::getVertices() const{
    return vertices;
}

const std::vector<glm::vec3>& Shape::getNormals() const{
    return normals;
}

const std::vector<glm::vec2>& Shape::getUvs() const{
    return uvs;
}
