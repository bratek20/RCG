

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Color.h"

#include <GL/glew.h>
#include <memory>
#include <string>

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class Texture {
    unsigned int id;
    std::string path;
    std::string type;

    unsigned char* data;
    int width;
    int height;
    GLenum format;
    int nrComponents;

  public:
    static TexturePtr create(const std::string &path,
                             const std::string &type);
    ~Texture();
    GLuint getId() const;
    const std::string &getPath() const;
    const std::string &getType() const;
    Color getColor(float u, float v) const;

  private:
    Texture(const std::string &path, const std::string &type);
    bool loadData(const std::string& fullPath);
    void loadTexture();
    Color toColor(unsigned char c) const;
};
#endif
