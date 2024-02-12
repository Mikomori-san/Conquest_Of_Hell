//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../GameObject.h"
#include <string>

class GameObject;

class Component
{
public:
    Component(std::weak_ptr<GameObject> gameObject, std::string id) : m_gameObject(gameObject), m_componentId(id) {}

    virtual ~Component() = default;
    virtual void update(float deltaTime) = 0;
    virtual std::string getComponentId() = 0;
    virtual void setComponentId(std::string id) = 0;
    virtual void init() = 0;
private:

protected:
    std::weak_ptr<GameObject> m_gameObject;
    std::string m_componentId;
};