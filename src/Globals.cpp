#include "Globals.h"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <limits>

float Globals::INF = std::numeric_limits<float>::max();
float Globals::previousFrameTime;
float Globals::currentFrameTime;
float Globals::deltaTime;
bool Globals::debug;

void Globals::init(const Config& c){
    currentFrameTime = glfwGetTime();
	deltaTime = 1.0f / 60.0f; 
	previousFrameTime = currentFrameTime - deltaTime;
    debug = c.debugMode;
}

void Globals::updateTime(){
    previousFrameTime = currentFrameTime;
    currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - previousFrameTime; 
}

float Globals::random(float a, float b){
    return a + (b - a) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}