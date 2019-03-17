

#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Color.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Material {
    unsigned int id;
    Color ambient;
    Color diffuse;
    Color specular;
    float ns;

    Material(aiMaterial* mat, unsigned int id);
    void apply(Shader& shader);

private:
    Color getColor(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int index, Color defaultCol = Colors::WHITE);
    float getFloat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int index, float defaultVal = 0.0f);
};

struct Vertex {
    static const glm::vec3 NORMAL_NOT_SET;

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    //glm::vec3 tangent;
    //glm::vec3 bitangent;

    Vertex(aiMesh *mesh, int idx);

    bool hasNormal() const;

private:
    glm::vec3 toVec3(aiVector3D* vectors, int idx, glm::vec3 defaultVec);
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
    Material& mat;

    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, Material& mat);
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Material material;
    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, const Material& material);

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

