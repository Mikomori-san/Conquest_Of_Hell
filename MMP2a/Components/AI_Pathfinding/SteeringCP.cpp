//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../../Math/VectorAlgebra2D.h"
#include "../Transformation_Components/TransformationCP.h"
#include "SteeringCP.h"


void SteeringCP::update(float deltaTime)
{
    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();

        std::shared_ptr<TransformationCP> trans = go->getComponentsOfType<TransformationCP>().at(0);
        sf::Vector2f pos = trans->getPosition();
        sf::Vector2f dir;
        
        if (destination != pos)
        {
            dir = MathUtil::unitVector(destination - pos);
            if (trans->getVelToggle())
                trans->toggleVelLock();
        }
        else
        {
            dir = sf::Vector2f(1, 1);
            trans->setVelocity(0);
            if(!trans->getVelToggle())
                trans->toggleVelLock();
        }   

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
