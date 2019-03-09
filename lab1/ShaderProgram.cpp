#include "ShaderProgram.h"

#include <common/shader.hpp>
#include <iostream>

using namespace std;

ShaderProgram::ShaderProgram(string vsName, string fsName){
    id = LoadShaders(vsName.c_str(), fsName.c_str());
    use();
}

void ShaderProgram::clear(){
    glDeleteProgram(id);
}

void ShaderProgram::use(){
    glUseProgram(id);
}