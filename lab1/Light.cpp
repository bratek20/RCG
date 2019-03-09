#include "Light.h"

Light::Light() : Actor(nullptr) {
}

LightPtr Light::create(float power, Color color, glm::vec3 coefficients){
    auto light = LightPtr(new Light());
    light->power = power;
    light->color = color;
    light->coefficients = coefficients;
    Mesh::addLight(light);

    return light;
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