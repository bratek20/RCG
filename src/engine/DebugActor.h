#ifndef DEBUG_ACTOR_H
#define DEBUG_ACTOR_H

#include "Actor.h"

class DebugActor;
using DebugActorPtr = std::shared_ptr<DebugActor>;

class DebugActor : public Actor {
    static DebugActorPtr instance;

public:
    static DebugActorPtr create();
    static DebugActorPtr get();

    void drawLine(glm::vec3 v1, glm::vec3 v2);

protected:
    DebugActor();
};
#endif