#include "Globals.h"

#include <GLFW/glfw3.h>
#include <cstdlib>

float Globals::previousFrameTime;
float Globals::currentFrameTime;
float Globals::deltaTime;
ActorPtr Globals::player;

void Globals::init(){
    Globals::currentFrameTime = glfwGetTime();
	Globals::deltaTime = 1.0f / 60.0f; 
	Globals::previousFrameTime = Globals::currentFrameTime - Globals::deltaTime;
}

void Globals::updateTime(){
    Globals::previousFrameTime = Globals::currentFrameTime;
    Globals::currentFrameTime = glfwGetTime();
    Globals::deltaTime = Globals::currentFrameTime - Globals::previousFrameTime; 
}

float Globals::random(float a, float b){
    return a + (b - a) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}