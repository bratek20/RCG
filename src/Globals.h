#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>

class Actor;
using ActorPtr = std::shared_ptr<Actor>;

class Globals{
public:
    static float previousFrameTime;
    static float currentFrameTime;
    static float deltaTime;

    static void init();
    static void updateTime();

    static float random(float a = 0.0f, float b = 1.0f);

    static ActorPtr player;    
};

#endif