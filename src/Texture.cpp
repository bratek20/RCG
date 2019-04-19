#include "Texture.h"

#include <iostream>

using namespace std;

TexturePtr Texture::create(const string &path, const string &type) {
    return TexturePtr(new Texture(path, type));
}

Texture::Texture(const string &path, const string &type)
    : path(path), type(type) {
    if (!loadData(path)) {
        cerr << "Texture loading failed for path: " << path << endl;
        return;
    }
    loadTexture();
}

Texture::~Texture() { stbi_image_free(data); }

bool Texture::loadData(const std::string &fullPath) {
    data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);

    if (!data) {
        return false;
    }

    if (nrComponents == 1) {
        format = GL_RED;
    }
    else if (nrComponents == 3) {
        format = GL_RGB;
    } else if (nrComponents == 4) {
        format = GL_RGBA;
    }
    return true;
}

void Texture::loadTexture() {
    id = 0;
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Texture::getId() const{
    return id;
}

const std::string &Texture::getPath() const{
    return path;
}

const std::string &Texture::getType() const{
    return type;
}

Color Texture::getColor(float u, float v) const{
    int i = (1-v) * height;
    int j = u * width; 
    unsigned bytePerPixel = nrComponents;
    unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
    unsigned char r = pixelOffset[0];
    unsigned char g = pixelOffset[1];
    unsigned char b = pixelOffset[2];
    return Color(r, g, b);
}