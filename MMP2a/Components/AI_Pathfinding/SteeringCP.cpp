#include "stdafx.h"
#include "SteeringCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "../../VectorAlgebra2D.h"

void SteeringCP::update(float deltaTime)
{
    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();

        std::shared_ptr<TransformationCP> trans = go->getComponentsOfType<TransformationCP>().at(0);
        sf::Vector2f pos = trans->getPosition();
        sf::Vector2f dir = MathUtil::unitVector(destination - pos);
        dir.x = std::round(dir.x * 100.f) / 100.f;
        dir.y = std::round(dir.y * 100.f) / 100.f;
        trans->setDirection(dir);
    }
}

std::string SteeringCP::getComponentId()
{
    return this->componentId;
}

void SteeringCP::setComponentId(std::string id)
{
    this->componentId = id;
}

void SteeringCP::init()
{
}
