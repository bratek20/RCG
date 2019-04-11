

#ifndef MODEL_H
#define MODEL_H 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
using namespace std;

class Model;
using ModelPtr = std::shared_ptr<Model>;

class Model 
{
    vector<Texture> textures;
    vector<Material> materials;
    vector<Mesh> meshes;
    vector<TrianglePtr> triangles;
    string directory;
    bool debug;

public:
    static ModelPtr create(const string& path, bool debug);

    void draw(Shader shader);
    const vector<TrianglePtr>& getTriangles() const;
    
private:
    Model(const string& path, bool debug);
    bool loadModel(const string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    unsigned int textureFromFile(const char *path, const string &directory, bool gamma = false);
    Material getMaterial(const aiScene *scene, unsigned int idx);
};
#endif

