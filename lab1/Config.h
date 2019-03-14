#ifndef CONFIG_H
#define CONFIG_H

#include "Color.h"

#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>

struct LightConfig {
    glm::vec3 position;
    Color color;
    float intensity;
};

struct Config {
    std::string comment;
    std::string loadScenePath;
    std::string saveImagePath;
    int k;
    int xRes, yRes;
    glm::vec3 viewPoint;
    glm::vec3 lookAt;
    glm::vec3 up;
    float yView;
    std::vector<LightConfig> lights;

    bool load(const std::string& path);

private:
    glm::vec3 readVec3(std::ifstream& file);
    Color readColor(std::ifstream& file);
};

#endif