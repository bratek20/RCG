#include "DebugActor.h"

DebugActorPtr DebugActor::instance = nullptr;

DebugActor::DebugActor() : Actor(Model::createEmpty()) {}

DebugActorPtr DebugActor::create() {
    instance = DebugActorPtr(new DebugActor());
    return instance;
}

DebugActorPtr DebugActor::get() {
    return instance;
}

void DebugActor::drawLine(glm::vec3 v1, glm::vec3 v2) {
    Material mat;
    Mesh mesh({v1, v2}, {0, 1}, mat);
    getModel()->addMesh(mesh, false);
}
