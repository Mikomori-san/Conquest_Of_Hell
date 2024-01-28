#include "stdafx.h"
#include "GameObject.h"
#include <iostream>

void GameObject::addComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
}

void GameObject::update(float deltaTime) {
    for (auto& comp : components) {
        comp->update(deltaTime);
    }
}

void GameObject::init()
{
    for (auto& comp : components) {
        comp->init();
    }
}

std::shared_ptr<Component> GameObject::getComponent(std::string id)
{
    for (auto& comp : components)
    {
        if (comp->getComponentId() == id)
        {
            return comp;
        }
    }
    return nullptr;
}
