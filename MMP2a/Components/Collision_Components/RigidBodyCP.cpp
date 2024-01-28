#include "stdafx.h"
#include "RigidBodyCP.h"
#include "../Transformation_Components/TransformationCP.h"

void RigidBodyCP::setPosNotifyTransf(sf::Vector2f pos)
{
    this->position = pos;
    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();
        std::shared_ptr<TransformationCP> transCP = go->getComponentsOfType<TransformationCP>().at(0);
        transCP->setPosition(position);
    }
}

// RigidBody Vel => transform.direction * transform.velocity => vel = RigidBodyVel / x or y (what is not 0)
void RigidBodyCP::setVelNotifyTransf(sf::Vector2f vel)
{
    
    this->velocity = vel;

    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();
        std::shared_ptr<TransformationCP> transCP = go->getComponentsOfType<TransformationCP>().at(0);
        if (vel.x == 0)
        {
            transCP->setVelocity(abs(vel.y));
            transCP->setDirection(vel / abs(vel.y));
        }
        else
        {
            transCP->setVelocity(abs(vel.x));
            transCP->setDirection(vel / abs(vel.x));
        }
    }
}

void RigidBodyCP::onCollision(std::shared_ptr<GameObject> go2)
{
    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();

        /*
        if (go2->getId().find("Boundary") != std::string::npos)
        {
            float vel = velocity.x == 0 ? velocity.y : velocity.x;
            sf::Vector2f direction(0, 0);

            if (go2->getId().find("Left") != std::string::npos)
            {
                direction = sf::Vector2f(1, 0);
            }
            else if (go2->getId().find("Top") != std::string::npos)
            {
                direction = sf::Vector2f(0, 1);
            }
            else if (go2->getId().find("Right") != std::string::npos)
            {
                direction = sf::Vector2f(-1, 0);
            }
            else if (go2->getId().find("Bottom") != std::string::npos)
            {
                direction = sf::Vector2f(0, -1);
            }
        */
        
            std::shared_ptr<TransformationCP> transCP = go->getComponentsOfType<TransformationCP>().at(0);
            transCP->setVelocity(0);
            transCP->setOldPos();
            transCP->setPosResetTimer();
            //setVelNotifyTransf(direction * abs(vel));
        //}
    }
}