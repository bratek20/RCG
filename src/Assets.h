#ifndef ASSETS_H
#define ASSETS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>
#include <functional>

#include "Program3D.h"
#include "Shape.h"

using namespace std;

class Assets {
    static const vector<string> POSSIBLE_PATH_PREFIXES;

public:
    static Program3D program;
    
    static void init(); 
    static void clear();
    static string validPath(const string& path);

private:
    static bool isValidPath(const string& path);
};

#endif