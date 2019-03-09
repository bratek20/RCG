#ifndef LIGHT_H
#define LIGHT_H

#include "Actor.h"
#include "Color.h"

class Light;
using LightPtr = std::shared_ptr<Light>;
using WeakLightPtr = std::weak_ptr<Light>;

class Light : public Actor {
    float power;
    Color color;
    glm::vec3 coefficients;

public:
    static LightPtr create(float power = 50.0f, Color color = Colors::WHITE, glm::vec3 coefficients = glm::vec3(1, 0, 0));

    float getPower() const;
    Color getColor() const;
    glm::vec3 getCoefficients() const;

protected:
    Light();
};
#endif