

#ifndef MODEL_H
#define MODEL_H 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Model 
{
    vector<Texture> loadedTextures;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

public:
    Model(){}
    Model(const string& path, bool gamma = false);

    void draw(Shader shader);
    vector<Triangle> getTriangles();
    
private:
    bool loadModel(const string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    unsigned int textureFromFile(const char *path, const string &directory, bool gamma = false);

    glm::vec3 tryConvert(aiVector3D* vectors, int idx, glm::vec3 defaultVal = glm::vec3(1, 0, 0));

};
#endif

