#include "Material.h"

using namespace std;

Material::Material(aiMaterial *mat, unsigned int id, const vector<TexturePtr>& textures) : id(id), textures(textures) {
    ambient = getColor<Color>(mat, AI_MATKEY_COLOR_AMBIENT);
    diffuse = getColor<Color>(mat, AI_MATKEY_COLOR_DIFFUSE);
    specular = getColor<Color>(mat, AI_MATKEY_COLOR_SPECULAR);
    emissive = getColor<glm::vec3>(mat, AI_MATKEY_COLOR_EMISSIVE);
    ns = getFloat(mat, AI_MATKEY_SHININESS);
}

Material::Material(Color ambient, Color diffuse, Color specular, glm::vec3 emissive, float ns) :
    ambient(ambient), diffuse(diffuse), specular(specular), emissive(emissive), ns(ns)
{}

Color Material::getTextureColor(float u, float v) const {
    if(textures.empty()){
        return Colors::WHITE;
    }
    return textures[0]->getColor(u, v);
}

bool Material::isLightSource() const{
    return emissive.x > 0 || emissive.y > 0 || emissive.z > 0; 
}

float Material::getFloat(aiMaterial *mat, const char *pKey, unsigned int type,
                         unsigned int index, float defaultVal) {
    ai_real value;
    if (AI_SUCCESS == aiGetMaterialFloat(mat, pKey, type, index, &value)) {
        return value;
    }
    return defaultVal;
}

void Material::apply(Shader &shader) {
    shader.applyFloat("NS", ns);
    shader.applyColor("AmbientColor", ambient);
    shader.applyColor("DiffuseColor", diffuse);
    shader.applyColor("SpecularColor", specular);

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 +
                        i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i]->getType();
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number =
                std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number =
                std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number =
                std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(
            glGetUniformLocation(shader.getId(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i]->getId());
    }
    shader.applyFloat("SkipTexture", textures.empty());
}