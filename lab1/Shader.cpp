#include "Shader.h"

#include <common/shader.hpp>
#include <iostream>

using namespace std;

Shader::Shader(string vsName, string fsName){
    id = LoadShaders(vsName.c_str(), fsName.c_str());
    use();
}

void Shader::clear(){
    glDeleteProgram(id);
}

void Shader::use(){
    glUseProgram(id);
}

GLuint Shader::getId() const{
    return id;
}