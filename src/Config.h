#ifndef CONFIG_H
#define CONFIG_H

#include "Color.h"

#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

struct LightConfig {
    glm::vec3 position;
    Color color;
    float intensity;
};

struct CameraConfig {
    glm::vec3 viewPoint;
    glm::vec3 lookAt;
    glm::vec3 up;
    float yView;
    float velocity = 1.0f;
};

struct Config {
    std::string comment;
    std::string loadScenePath;
    std::string saveImagePath;
    int k;
    int xRes, yRes;
    Color background = Colors::BLACK;

    CameraConfig camera;

    glm::vec3 lightCoef = glm::vec3(1, 0, 0);
    std::vector<LightConfig> lights;

    bool debugMode = false;

    bool load(const std::string& path);

private:
    glm::vec3 readVec3(std::ifstream& file);
    Color readColor(std::ifstream& file);
};

#endif