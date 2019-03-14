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

    string type;
    while(file >> type)
    {
        if(type == "L")
        {
            LightConfig lc;
            lc.position = readVec3(file);
            lc.color = readColor(file);
            file >> lc.intensity;
            lights.push_back(lc);
        }
        else
        {
            cerr << "Type: " << type << " not supported!" << endl;
        }
        
    }
    return true;
}

glm::vec3 Config::readVec3(ifstream& file)
{
    float x, y, z;
    file >> x >> y >> z;
    return glm::vec3(x, y, z);
}

Color Config::readColor(std::ifstream& file)
{
    int r, g, b;
    file >> r >> g >> b;
    return Color(r, g, b);
}