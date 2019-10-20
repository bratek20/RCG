#include "Model.h"
#include "Assets.h"

using namespace std;

ModelPtr Model::create(const Config& c) {
    return ModelPtr(new Model(c));
}

ModelPtr Model::createEmpty() {
    return ModelPtr(new Model());
}

Model::Model(const Config& c) {
    string fullScenePath = Assets::validPath(c.loadScenePath);
    if (loadModel(fullScenePath)) {
        for(auto& lc : c.lights) {
            meshes.push_back(lc.createMesh());
        }
        createTriangles();
        cout << "Scene model: " << fullScenePath << " loaded!" << endl;
    } else {
        cerr << "Scene model: " << fullScenePath << " loading failed!" << endl;
    }
}

void Model::addMesh(const Mesh& mesh, bool rebuild) {
    meshes.push_back(mesh);
    if(rebuild) {
        createTriangles();
    }
}

void Model::clearMeshes() {
    meshes.clear();
}


// draws the model, and thus all its meshes
void Model::draw(Shader shader) {
    for (unsigned int i = 0; i < meshes.size(); i++){
        meshes[i].draw(shader);
    }
}

bool Model::loadModel(const string &path) {
    directory = path.substr(0, path.find_last_of('/'));
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs |
                  aiProcess_CalcTangentSpace | aiProcess_GenNormals);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) // if is Not Zero
    {

        cerr << "Error: " << importer.GetErrorString() << endl;
        cerr << "Given path: " << path << endl;
        return false;
    }

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
    return true;
}

void Model::createTriangles() {
    triangles.clear();
    for (auto &mesh : meshes) {
        auto &mTris = mesh.getTriangles();
        for (auto &tri : mTris) {
            triangles.push_back(&tri);
        }
    }
}

// processes a node in a recursive fashion. Processes each individual mesh
// located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in
        // the scene. the scene contains all the data, node is just to keep
        // stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively
    // process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    Material mat = getMaterial(scene, mesh->mMaterialIndex);
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex(mesh, i);
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle)
    // and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, mat);
}

Material Model::getMaterial(const aiScene *scene, unsigned int idx) {
    auto it = find_if(materials.begin(), materials.end(),
                      [&](const Material &mat) { return mat.id == idx; });
    if (it != materials.end()) {
        return *it;
    }

    vector<TexturePtr> textures;
        // process materials
    aiMaterial *material = scene->mMaterials[idx];
    // we assume a convention for sampler names in the shaders. Each diffuse
    // texture should be named as 'texture_diffuseN' where N is a sequential
    // number ranging from 1 to MAX_SAMPLER_NUMBER. Same applies to other
    // texture as the following list summarizes: diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<TexturePtr> diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<TexturePtr> specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<TexturePtr> normalMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<TexturePtr> heightMaps =
        loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    materials.emplace_back(scene->mMaterials[idx], idx, textures);
    return materials.back();
}

// checks all material textures of a given type and loads the textures if
// they're not loaded yet. the required info is returned as a Texture struct.
vector<TexturePtr> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            string typeName) {
    vector<TexturePtr> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString name;
        mat->GetTexture(type, i, &name);
        string fullPath = directory + "/" + name.C_Str();
        // check if texture was loaded before and if so, continue to next
        // iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures.size(); j++) {
            if (textures[j]->getPath() == fullPath) {
                textures.push_back(textures[j]);
                skip =
                    true; // a texture with the same filepath has already been
                          // loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip) { // if texture hasn't been loaded already, load it
            textures.push_back(Texture::create(fullPath, typeName));
        }
    }
    return textures;
}

const vector<TrianglePtr> &Model::getTriangles() const { return triangles; }
const vector<LightConfig> &Model::getLights() const { return lights; }
const vector<Mesh> &Model::getMeshes() const { return meshes; }

