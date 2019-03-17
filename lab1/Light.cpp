#include "Light.h"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

const int Light::MAX_LIGHTS = 10;
std::vector<LightPtr> Light::lights;

Light::Light() : Actor(nullptr) {
}

LightPtr Light::create(float power, Color color, glm::vec3 coefficients){
    auto light = LightPtr(new Light());
    light->power = power;
    light->color = color;
    light->coefficients = coefficients;
    addLight(light);

    return light;
}

void Light::loadLights(const Config& c){
    for(auto& lc : c.lights){
        auto light = create(lc.intensity, lc.color, c.lightCoef);
        light->setPosition(lc.position);
    }
}

void Light::applyLights(const Shader& s){
    GLuint id = s.getId();
    GLuint numId = glGetUniformLocation(id, "LightsNum");
    GLuint posId = glGetUniformLocation(id, "LightPosition_worldspace");
    GLuint powerId = glGetUniformLocation(id, "LightPower");
    GLuint colorId = glGetUniformLocation(id, "LightColor");
    GLuint coefficientsId = glGetUniformLocation(id, "LightDistanceCoefficients");

    int size = std::min(static_cast<int>(lights.size()), MAX_LIGHTS);
    glUniform1i(numId, size);   

    glm::vec3 pos[size];
    float power[size];
    glm::vec3 color[size];
    glm::vec3 coefficient[size];
    for(int i=0;i<size;i++){
        auto light = lights[i];
        pos[i] = light->getWorldPosition();
        power[i] = light->getPower();
        color[i] = static_cast<glm::vec3>(light->getColor());
        coefficient[i] = light->getCoefficients();
    }

    glUniform3fv(posId, size, glm::value_ptr(pos[0]));
    glUniform1fv(powerId, size, power);
    glUniform3fv(colorId, size, glm::value_ptr(color[0]));
    glUniform3fv(coefficientsId, size, glm::value_ptr(coefficient[0]));
}

void Light::addLight(LightPtr light){
    lights.push_back(light);
}

const std::vector<LightPtr>& Light::getLights(){
    return lights;
}

float Light::getPower() const{
    return power;
}

Color Light::getColor() const{
    return color;
}

glm::vec3 Light::getCoefficients() const{
    return coefficients;
}