#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <glm/glm.hpp>
#include <fstream>

struct Config{
    std::string comment;
    std::string loadScenePath;
    std::string saveImagePath;
    int k;
    int xRes, yRes;
    glm::vec3 viewPoint;
    glm::vec3 lookAt;
    glm::vec3 up;
    float yView;

    bool load(const std::string& path);

private:
    glm::vec3 readVec3(std::ifstream& file);
};

#endif