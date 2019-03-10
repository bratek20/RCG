#include "Config.h"

#include <iostream>

using namespace std;

bool Config::load(const string& path)
{
    ifstream file(path);
    if(!file.is_open())
    {
        cerr << "File: " << path << " not opened!"; 
        return false;
    }

    getline(file, comment);
    getline(file, loadScenePath);
    getline(file, saveImagePath);
    file >> k;
    file >> xRes >> yRes;
    viewPoint = readVec3(file);
    lookAt = readVec3(file);
    up = readVec3(file);
    file >> yView;

    return true;
}

glm::vec3 Config::readVec3(ifstream& file)
{
    float x, y, z;
    file >> x >> y >> z;
    return glm::vec3(x, y, z);
}