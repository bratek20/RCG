#ifndef LIGHT_H
#define LIGHT_H

#include "Actor.h"
#include "Color.h"
#include "Shader.h"
#include "Config.h"

class Light;
using LightPtr = std::shared_ptr<Light>;
using WeakLightPtr = std::weak_ptr<Light>;

class Light : public Actor {
    static const int MAX_LIGHTS;
    static std::vector<LightPtr> lights;

    float power;
    Color color;
    glm::vec3 coefficients;
    
public:
    static void loadLights(const std::vector<LightConfig>& lights);
    static LightPtr create(float power = 50.0f, Color color = Colors::WHITE, glm::vec3 coefficients = glm::vec3(1, 0, 0));
    static void applyLights(const Shader& s);

    static const std::vector<LightPtr>& getLights();

    float getPower() const;
    Color getColor() const;
    glm::vec3 getCoefficients() const;

protected:
    Light();

private:
    static void addLight(LightPtr light);
};
#endif