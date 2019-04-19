

#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Color.h"
#include "Utils.h"
#include "Texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Material {
    unsigned int id;
    Color ambient;
    Color diffuse;
    Color specular;
    float ns;
    std::vector<TexturePtr> textures;

    Material() = default;
    Material(aiMaterial* mat, unsigned int id, const std::vector<TexturePtr>& textures);
    void apply(Shader& shader);

    Color getTextureColor(float u, float v) const;

private:
    Color getColor(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int index, Color defaultCol = Colors::WHITE);
    float getFloat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int index, float defaultVal = 0.0f);
};

struct Vertex {
    static const glm::vec3 NORMAL_NOT_SET;
    static const glm::vec2 UV_NOT_SET;

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    //glm::vec3 tangent;
    //glm::vec3 bitangent;

    Vertex() = default;
    Vertex(aiMesh *mesh, int idx);

    bool hasNormal() const;
    bool hasUV() const;
private:
    glm::vec3 toVec3(aiVector3D* vectors, int idx, glm::vec3 defaultVec);
    glm::vec2 toVec2(aiVector3D *vectors, int idx, glm::vec2 defaultVec); 
};

struct Bounds {
    static const glm::vec3 MIN_NOT_SET;
    static const glm::vec3 MAX_NOT_SET;

    glm::vec3 pMin;
    glm::vec3 pMax;

    Bounds();
    Bounds(const std::vector<glm::vec3>& poses);
    Bounds(glm::vec3 pMin, glm::vec3 pMax) :
        pMin(pMin), pMax(pMax) {}

    Bounds merge(const Bounds& other) const;
    std::pair<Bounds, Bounds> split(Utils::Axis axis, float value) const;

    bool isValid() const;
    glm::vec3 diagonal() const;
    float surfaceArea() const;
    Utils::Axis maximumExtent() const;
};

struct Triangle {
    int id;
    Vertex v1;
    Vertex v2;
    Vertex v3;
    Material mat;
    Bounds bounds;

    Triangle() = default;
    Triangle(Vertex& v1, Vertex& v2, Vertex& v3, const Material& mat);

    glm::vec3 getNormal(glm::vec2 baryPos) const;
    glm::vec3 getNormal() const;
    glm::vec2 getUV(glm::vec2 baryPos) const;
    std::vector<glm::vec3> getPositions() const;
    Bounds getBounds() const;
};
using TrianglePtr = const Triangle*;

class Mesh {
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Triangle> triangles;
    Material material;
    unsigned int VAO;
    bool debug;
    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const Material& material, bool debug);

    // render the mesh
    void draw(Shader& shader);
    const std::vector<Triangle>& getTriangles() const;

private:
    /*  Render data  */
    unsigned int VBO, EBO;

    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh();
};
#endif

