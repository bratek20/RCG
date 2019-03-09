#include "Assets.h"
#include "Light.h"

#include <common/shader.hpp>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

GLuint Assets::vertexArrayIdx;

ShapePtr Assets::CUBE;
ShapePtr Assets::SPHERE;
ShapePtr Assets::OBJ;
GLuint Assets::WHITE_TEXTURE;
GLuint Assets::TEXTURE;
GLuint Assets::GLASS_TEXTURE;
GLuint Assets::NOISE_TEXTURE;

void Assets::init(){
    glGenVertexArrays(1, &vertexArrayIdx);
    glBindVertexArray(vertexArrayIdx);  
    
    WHITE_TEXTURE = loadBMP_custom("assets/whiteTexture.bmp");
    TEXTURE = loadDDS("assets/uvmap.DDS");
    GLASS_TEXTURE = loadBMP_custom("assets/glass.bmp");
    NOISE_TEXTURE = loadBMP_custom("assets/perlinNoise.bmp");

    CUBE = loadShapeFromObj("assets/cube.obj");
    SPHERE = loadShapeFromObj("assets/sphere.obj");
    OBJ = loadShapeFromObj("assets/suzanne.obj");
}

void Assets::clear(){
    glDeleteVertexArrays(1, &vertexArrayIdx);
}

ShapePtr Assets::loadShapeFromObj(std::string path){
    std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

    bool res = loadOBJ(path.c_str(), vertices, uvs, normals);
    if(!res){
        cout << "Loading " << path << " failed!" << endl;
        return nullptr;
    }

    normalize(vertices);
    return Shape::create(vertices, normals, uvs);
}

void Assets::normalize(std::vector<glm::vec3>& data){
    float distX = maxDist(data, [](glm::vec3 vec){return vec.x;});
    float distY = maxDist(data, [](glm::vec3 vec){return vec.y;});
    float distZ = maxDist(data, [](glm::vec3 vec){return vec.z;}); 
    float dist = max({distX, distY, distZ});
    for(auto& vec : data){
        vec /= dist;
    }
}

std::vector<glm::vec3> Assets::toVec3(const std::vector<float>& data){
    std::vector<glm::vec3> vec3Data;
    for(unsigned i=0;i<data.size();i+=3){
        vec3Data.push_back({data[i], data[i+1], data[i+2]});
    }
    return vec3Data;
}

float Assets::maxDist(const std::vector<glm::vec3>& data, std::function<float(glm::vec3)> valueGetter){
    float minVal = 1e9;
    float maxVal = -1e9;
    for(glm::vec3 vec : data){
        float val = valueGetter(vec);
        minVal = min(minVal, val);
        maxVal = max(maxVal, val);
    }
    return maxVal - minVal;
}

