

#include "Mesh.h"
#include <iostream>
const glm::vec3 Vertex::NORMAL_NOT_SET = glm::vec3(0);

Material::Material(aiMaterial *mat, unsigned int id) : id(id)
{
    ambient = getColor(mat, AI_MATKEY_COLOR_AMBIENT);
    diffuse = getColor(mat, AI_MATKEY_COLOR_DIFFUSE);
    specular = getColor(mat, AI_MATKEY_COLOR_SPECULAR);
    ns = getFloat(mat, AI_MATKEY_SHININESS);
}

Color Material::getColor(aiMaterial *mat, const char *pKey, unsigned int type, unsigned int index, Color defaultCol)
{
    aiColor4D color;
    if (AI_SUCCESS == aiGetMaterialColor(mat, pKey, type, index, &color))
    {
        return Color(color.r, color.g, color.b);
    }
    return defaultCol;
}

float Material::getFloat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int index, float defaultVal)
{
    ai_real value;
    if (AI_SUCCESS == aiGetMaterialFloat(mat, pKey, type, index, &value))
    {
        return value;
    }
    return defaultVal;
}

void Material::apply(Shader& shader)
{
    shader.applyFloat("NS", ns);
    shader.applyColor("AmbientColor", ambient);
    shader.applyColor("DiffuseColor", diffuse);
    shader.applyColor("SpecularColor", specular);
}


Vertex::Vertex(aiMesh *mesh, int idx)
{
    position = toVec3(mesh->mVertices, idx, glm::vec3(0));
    normal = toVec3(mesh->mNormals, idx, NORMAL_NOT_SET);

    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
        glm::vec2 vec;
        // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
        // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
        //vec.x = mesh->mTextureCoords[0][idx].x;
        //vec.y = mesh->mTextureCoords[0][idx].y;
        texCoords = vec;
    }
    else
    {
        texCoords = glm::vec2(0.0f, 0.0f);
    }
}

glm::vec3 Vertex::toVec3(aiVector3D *vectors, int idx, glm::vec3 defaultVec)
{
    if (vectors != NULL)
    {
        return glm::vec3(vectors[idx].x, vectors[idx].y, vectors[idx].z);
    }
    return defaultVec;
}

bool Vertex::hasNormal() const
{
    return normal != NORMAL_NOT_SET;
}

Triangle::Triangle(Vertex &v1, Vertex &v2, Vertex &v3, const Material& mat) : 
    v1(v1), v2(v2), v3(v3), mat(mat)
{
    static int nextId = 1;
    id = nextId++;
}

glm::vec3 Triangle::getNormal(glm::vec2 baryPos) const
{
    if (v1.hasNormal() && v2.hasNormal() && v3.hasNormal())
    {
        return (1 - baryPos.x - baryPos.y) * v1.normal + baryPos.x * v2.normal + baryPos.y * v3.normal;
    }
    else
    {
        return getNormal();
    }
}

glm::vec3 Triangle::getNormal() const
{
    return glm::normalize(glm::cross(v2.position - v1.position, v3.position - v1.position));
}

vector<glm::vec3> Triangle::getPositions() const {
    return {v1.position, v2.position, v3.position};
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, const Material& material) :
    vertices(vertices), indices(indices), textures(textures), material(material)
{
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

void Mesh::draw(Shader &shader)
{
    material.apply(shader);

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.getId(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}
void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
    // // vertex tangent
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // // vertex bitangent
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);

    triangles.clear();
    for (int i = 0; i < indices.size(); i += 3)
    {
        triangles.emplace_back(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]], material);
    }
}

const vector<Triangle>& Mesh::getTriangles() const
{
    return triangles;
}
