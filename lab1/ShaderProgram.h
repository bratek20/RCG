#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <string>

class ShaderProgram {
protected:
    GLuint id;

public:
    ShaderProgram() {}
    ShaderProgram(std::string vsName, std::string fsName);

    void clear();
    void use();    
    
};

#endif