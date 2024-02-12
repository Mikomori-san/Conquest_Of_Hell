//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "GameObject.h"
#include <iostream>

void GameObject::addComponent(std::shared_ptr<Component> component) {
    m_components.push_back(component);
}

void GameObject::update(float deltaTime) {
    for (auto& comp : m_components) {
        comp->update(deltaTime);
    }
}

void GameObject::init()
{
    for (auto& comp : m_components) {
        comp->init();
    }
}

std::shared_ptr<Component> GameObject::getComponent(std::string id)
{
    for (auto& comp : m_components)
    {
        if (comp->getComponentId() == id)
        {
            return comp;
        }
    }
    return nullptr;
}
