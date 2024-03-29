//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "Collision_Components/RigidBodyCP.h"
#include "DecisionHandlerCP.h"
#include "../Enums/Direction.h"
#include "Graphics_Components/AnimatedGraphicsCP.h"
#include "Transformation_Components/TransformationCP.h"

std::string DecisionHandlerCP::getComponentId()
{
    return this->m_componentId;
}

void DecisionHandlerCP::handleMovement(Direction dir, sf::Vector2f dirVec, float vel)
{
    if (!m_gameObject.expired())
    {
        std::shared_ptr<GameObject> go = m_gameObject.lock();
        std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> aniGraphics = go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0);
        std::shared_ptr<TransformationCP> transformation = go->getComponentsOfType<TransformationCP>().at(0);
        std::shared_ptr<RigidBodyCP> rigidBody = go->getComponentsOfType<RigidBodyCP>().at(0);

        transformation->setVelocity(0);

        switch (dir)
        {
        case left:
            aniGraphics->setAnimationType(Player_Animationtype::Left);
            break;
        case right:
            aniGraphics->setAnimationType(Player_Animationtype::Right);
            break;
        case upRight:
            aniGraphics->setAnimationType(Player_Animationtype::Right);
            break;
        case rightDown:
            aniGraphics->setAnimationType(Player_Animationtype::Right);
            break;
        case downLeft:
            aniGraphics->setAnimationType(Player_Animationtype::Left);
            break;
        case leftUp:
            aniGraphics->setAnimationType(Player_Animationtype::Left);
            break;
        default:
            break;
        }

        transformation->setDirection(dirVec);
        transformation->setVelocity(vel);
        rigidBody->setVel(transformation->getDirection() * transformation->getVelocity());
    }
}

void DecisionHandlerCP::handleIdle(Direction dir)
{
    if (!m_gameObject.expired())
    {
        std::shared_ptr<GameObject> go = m_gameObject.lock();
        std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> aniGraphics = go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0);

        switch (dir)
        {
        case left:
            aniGraphics->setAnimationType(Player_Animationtype::LeftIdle);
            break;
        case right:
            aniGraphics->setAnimationType(Player_Animationtype::RightIdle);
            break;
        case upRight:
            aniGraphics->setAnimationType(Player_Animationtype::RightIdle);
            break;
        case rightDown:
            aniGraphics->setAnimationType(Player_Animationtype::RightIdle);
            break;
        case downLeft:
            aniGraphics->setAnimationType(Player_Animationtype::LeftIdle);
            break;
        case leftUp:
            aniGraphics->setAnimationType(Player_Animationtype::LeftIdle);
            break;
        default:
            break;
        }
    }
}