#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Color.h"
#include "Texture.h"

#include <string>
#include <vector>

struct Material {
    unsigned int id;
    Color ambient;
    Color diffuse;
    Color specular;
    glm::vec3 emissive;
    float ns;
    std::vector<TexturePtr> textures;

    Material() = default;
    Material(aiMaterial* mat, unsigned int id, const std::vector<TexturePtr>& textures);
    Material(Color ambient, Color diffuse, Color specular, glm::vec3 emissive, float ns);
    void apply(Shader& shader);

    Color getTextureColor(float u, float v) const;
    bool isLightSource() const;
    
private:
    template<typename T>
    T getColor(aiMaterial *mat, const char *pKey, unsigned int type,
                            unsigned int index) {
        aiColor4D color;
        if (AI_SUCCESS == aiGetMaterialColor(mat, pKey, type, index, &color)) {
            return T(color.r, color.g, color.b);
        }
        return T(0.0f, 0.0f, 0.0f);
    }

    float getFloat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int index, float defaultVal = 0.0f);
};

#endif
