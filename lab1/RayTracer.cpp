#include "RayTracer.h"

#include <cstdio>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

std::pair<bool, Color> RayTracer::cast(int k, glm::vec3 origin, glm::vec3 direction, const std::vector<Triangle> &triangles, const std::vector<LightPtr> &lights)
{
    const Triangle *ansTri = NULL;
    float ansDist = std::numeric_limits<float>::max();
    glm::vec2 ansBaryPos;

    for (auto &tri : triangles)
    {
        glm::vec2 baryPos;
        float distance;
        bool intersetcs = glm::intersectRayTriangle(origin, direction, tri.v1.position, tri.v2.position, tri.v3.position, baryPos, distance);
        if (intersetcs && distance < ansDist)
        {
            ansTri = &tri;
            ansDist = distance;
            ansBaryPos = baryPos;
        }
    }
    
    if (ansTri == NULL)
    {
        return std::make_pair(false, Colors::BLACK);
    }

    if(k==0)
    {
        return std::make_pair(true, ansTri->v1.color);
    }
    
    glm::vec3 intersec = origin + direction * ansDist;
    glm::vec3 reflectDir = glm::normalize(glm::reflect(glm::normalize(intersec), ansTri->normal));
    float reflectParam = 0.0f;
    pair<bool, Color> reflectCast = cast(k-1, intersec, reflectDir, triangles, lights); 
    Color phongColor = phongShading(intersec, ansTri->normal, direction, ansTri->v1.color, triangles, lights); 
    return std::make_pair(true, phongColor + reflectCast.second * reflectParam);
} 

Color RayTracer::phongShading(glm::vec3 position, glm::vec3 normal, glm::vec3 rayDirection, Color vertexColor, const std::vector<Triangle> &triangles, const std::vector<LightPtr> &lights)
{
    Color color = Colors::BLACK;
    Color materialDiffuseColor = vertexColor;
	Color materialAmbientColor = Color(0.2f,0.2f,0.2f) * materialDiffuseColor; 
	Color materialSpecularColor = Color(0.005f,0.005f,0.005f);
	for(auto& light : lights){
		// Distance to the light
        glm::vec3 lightDir = light->getWorldPosition() - position;
        float distance = length(lightDir);
        lightDir = glm::normalize(lightDir);
        cast(0, position, -lightDir, triangles, lights);
		

		// Cosine of the angle between the normal and the light direction, 
		// clamped above 0
		//  - light is at the vertical of the triangle -> 1
		//  - light is perpendicular to the triangle -> 0
		//  - light is behind the triangle -> 0
        glm::vec3 l = glm::normalize(lightDir);
		float cosTheta = glm::clamp( glm::dot( normal, l), 0.0f, 1.0f);
		
		// Eye vector (towards the camera)
		// Direction in which the triangle reflects the light
		glm::vec3 reflectedL = glm::normalize(reflect(-l, normal));
		// Cosine of the angle between the Eye vector and the Reflect vector,
		// clamped to 0
		//  - Looking into the reflection -> 1
		//  - Looking elsewhere -> < 1
		float cosAlpha = glm::clamp( glm::dot( normalize(-rayDirection), reflectedL), 0.0f, 1.0f ); 
        auto coeff = light->getCoefficients();
		float distanceLoss = 
			distance * distance * coeff.x +
			distance * coeff.y +
			coeff.z;  
		color += 
			// Ambient : simulates indirect lighting
			materialAmbientColor
			// Diffuse : "color" of the object
			+ materialDiffuseColor * light->getColor() * light->getPower() * cosTheta / distanceLoss
			// Specular : reflective highlight, like a mirror
			+ materialSpecularColor * light->getColor() * light->getPower() * pow(cosAlpha, 16) / distanceLoss;
	}

    return color;
}