#include "Random.h"
#include "Mesh.h"

std::default_random_engine Random::generator;

float Random::uniform(float a, float b){
    std::uniform_real_distribution<double> distribution(a, b);
    return distribution(generator);
}

glm::vec3 Random::vectorOnHemisphere(glm::vec3 normal){
    float nPhi = atan2(normal.y, normal.x);
    float nTheta = acos(normal.z);

    float theta = nTheta + acos(1 - uniform());
    float phi = nPhi + uniform(0, 2 * M_PI);
    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);
    return glm::vec3(x, y, z);
}

glm::vec3 Random::pointInTriangle(TrianglePtr triangle){
    while(true){
        float a = uniform();
        float b = uniform();
        glm::vec3 point = a * triangle->getPos12() + b * triangle->getPos13();
        if(triangle->isInside(point)){
            return point;
        }
    }
}
