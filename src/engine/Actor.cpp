#include "Actor.h"
#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Assets.h"

using namespace std;

Actor::Actor(ModelPtr model) : position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.0f), model(model) {
}

ActorPtr Actor::create(ModelPtr model){
    auto actor = ActorPtr(new Actor(model));
    return actor;
}

void Actor::destroy(){
    exists = false;
}

void Actor::update(){
    for(auto& c : childs){
        c->update();
    }

    onUpdate();

    childs.erase(
        std::remove_if(childs.begin(), childs.end(), [](ActorPtr a){return !a->exists;}),
        childs.end());
}

void Actor::render(const glm::mat4& worldMat){
    auto myWorldMat = worldMat * getLocalMat();
    if(model != nullptr && isVisible){
        Assets::program.applyWorldMat(myWorldMat);
        model->draw(Assets::program);
    }

    for(auto& c : childs){
        c->render(myWorldMat);
    }
}

void Actor::setVisibleStatus(bool isVisible){
    this->isVisible = isVisible;
}

void Actor::addChild(ActorPtr child){
    child->parent = shared_from_this();
    childs.push_back(child);
}

void Actor::addChilds(std::vector<ActorPtr> childs){
    for(auto newChild : childs){
        addChild(newChild);
    }
}

int Actor::childsNum() const{
    return childs.size();
}

std::vector<ActorPtr> Actor::getChilds() const{
    return childs;
}

void Actor::setPosition(glm::vec3 position){
    this->position = position;
}

void Actor::setScale(glm::vec3 scale){
    this->scale = scale;
}

void Actor::setRotation(glm::vec3 rotation){
    this->rotation = rotation;   
}

void Actor::move(glm::vec3 dPos){
    position += dPos;
}

void Actor::rotate(glm::vec3 dRot){
    rotation += dRot;
}

ModelPtr Actor::getModel() const{
    return model;
}

glm::vec3 Actor::getLocalPosition() const{
    return position;
}

glm::vec3 Actor::getWorldPosition() const{
    return static_cast<glm::vec3>(getWorldMat() * glm::vec4(0, 0, 0, 1));
}

glm::vec3 Actor::getScale() const{
    return scale;
}

glm::vec3 Actor::getRotation() const{
    return rotation;
}

bool Actor::isDestroyed() const{
    return exists;
}

glm::mat4 Actor::getWorldMat() const{
    if(parent.expired()){
        return getLocalMat();
    }
    return parent.lock()->getWorldMat() * getLocalMat();
}

glm::mat4 Actor::getLocalMat() const{
    return getPositionMat() * getRotationMat() * getScaleMat();
}

glm::mat4 Actor::getPositionMat() const{
    return glm::translate(glm::mat4(1.0f), position);
}

glm::mat4 Actor::getScaleMat() const{
    return glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Actor::getRotationMat() const{
    static const glm::vec3 xAxis = glm::vec3(1,0,0);
    static const glm::vec3 yAxis = glm::vec3(0,1,0);
    static const glm::vec3 zAxis = glm::vec3(0,0,1);

    glm::mat4 rotationMat(1.0f);
    glm::quat rotZ = glm::angleAxis(glm::radians(rotation.z), zAxis);
    glm::quat rotY = glm::angleAxis(glm::radians(rotation.y), yAxis);
    glm::quat rotX = glm::angleAxis(glm::radians(rotation.x), xAxis);
    
    rotationMat = glm::toMat4(rotZ * rotY * rotX);
    return rotationMat;
}