

#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Color.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    Color color;
    glm::vec2 texCoords;
    //glm::vec3 tangent;
    //glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Triangle {
    Vertex v1;
    Vertex v2;
    Vertex v3;
    glm::vec3 normal;

    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    // render the mesh
    void draw(Shader& shader);
    vector<Triangle> getTriangles();

private:
    /*  Render data  */
    unsigned int VBO, EBO;

    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh();
};
#endif

