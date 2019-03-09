#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader {
protected:
    GLuint id;

public:
    Shader() {}
    Shader(std::string vsName, std::string fsName);

    void clear();
    void use();    
    GLuint getId() const;
};

#endif