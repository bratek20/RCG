#include "Random.h"
#include "Mesh.h"

std::default_random_engine Random::generator;

float Random::uniform(float a, float b){
    std::uniform_real_distribution<double> distribution(a, b);
    return distribution(generator);
}

bool Random::tossCoin(float winProbability){
    return uniform() < winProbability;
}

glm::vec3 Random::vectorOnHemisphereUniform(glm::vec3 normal){
    float theta = acos(1 - uniform());
    float phi = uniform(0, 2 * M_PI);
    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);
    return rotateToGlobalSpace(glm::vec3(x, y, z), normal);
}

glm::vec3 Random::vectorOnHemisphereCos(glm::vec3 normal){
    float x, z;
    do{
        x = uniform(-1, 1);
        z = uniform(-1, 1);
    }while(x*x + z*z > 1);

    float y = sqrt(1-x*x-z*z);
    return rotateToGlobalSpace(glm::vec3(x, y, z), normal);
}

glm::vec3 Random::pointInTriangle(TrianglePtr triangle){
    float a,b;
    do{
        a = uniform();
        b = uniform();
    } while(a + b > 1);

    return triangle->v1.position * a + triangle->v2.position * b  + triangle->v3.position * (1-a-b); 
}

glm::mat3 rotateAxisAngle( glm::vec3 u, float angleRadians )
{
    const float sinA = sinf( angleRadians );
    const float cosA = cosf( angleRadians );
    const float invCosA = 1.0f - cosA;

    return glm::mat3( (u.x * u.x * invCosA) + cosA,
                 (u.y * u.x * invCosA) - (sinA * u.z), 
                 (u.z * u.x * invCosA) + (sinA * u.y),
                 (u.x * u.y * invCosA) + (sinA * u.z),  
                 (u.y * u.y * invCosA) + cosA,      
                 (u.z * u.y * invCosA) - (sinA * u.x),
                 (u.x * u.z * invCosA) - (sinA * u.y),  
                 (u.y * u.z * invCosA) + (sinA * u.x),  
                 (u.z * u.z * invCosA) + cosA 
                 );
}

glm::mat3 rotateAlign(glm::vec3 u1, glm::vec3 u2)
{
    glm::vec3 axis = normalize( cross( u1, u2 ) );
    float dotProduct = dot( u1, u2 );
    dotProduct = glm::clamp(dotProduct, -1.0f, 1.0f );
    float angleRadians = acosf( dotProduct );
    glm::mat3 result = rotateAxisAngle( axis, angleRadians );
    return result;
}

glm::vec3 Random::rotateToGlobalSpace(glm::vec3 vec, glm::vec3 globalNormal){
    return vec;// * rotateAlign(glm::vec3(0, 1, 0), globalNormal);
}
