#include "stdafx.h"
#include "DecisionHandlerCP.h"
#include "../Enums/Direction.h"
#include "Graphics_Components/AnimatedGraphicsCP.h"
#include "Transformation_Components/TransformationCP.h"
#include "Collision_Components/RigidBodyCP.h"

void DecisionHandlerCP::update(float deltaTime)
{

}

std::string DecisionHandlerCP::getComponentId()
{
    return this->componentId;
}

void DecisionHandlerCP::setComponentId(std::string id)
{

}

void DecisionHandlerCP::init()
{

}

void DecisionHandlerCP::handleMovement(Direction dir, sf::Vector2f dirVec, float vel)
{
    std::shared_ptr<GameObject> go = gameObject.lock();
    std::shared_ptr<AnimatedGraphicsCP> aniGraphics = go->getComponentsOfType<AnimatedGraphicsCP>().at(0);
    std::shared_ptr<TransformationCP> transformation = go->getComponentsOfType<TransformationCP>().at(0);
    std::shared_ptr<RigidBodyCP> rigidBody = go->getComponentsOfType<RigidBodyCP>().at(0);

    transformation->setVelocity(0);

    switch (dir)
    {
    case up:
        aniGraphics->setAnimationType(Animationtype::Up);
        break;
    case down:
        aniGraphics->setAnimationType(Animationtype::Down);
        break;
    case left:
        aniGraphics->setAnimationType(Animationtype::Left);
        break;
    case right:
        aniGraphics->setAnimationType(Animationtype::Right);
        break;
    case upRight:
        aniGraphics->setAnimationType(Animationtype::UpRight);
        break;
    case rightDown:
        aniGraphics->setAnimationType(Animationtype::RightDown);
        break;
    case downLeft:
        aniGraphics->setAnimationType(Animationtype::DownLeft);
        break;
    case leftUp:
        aniGraphics->setAnimationType(Animationtype::LeftUp);
        break;
    default:
        break;
    }

    transformation->setDirection(dirVec);
    transformation->setVelocity(vel);
    rigidBody->setVel(transformation->getDirection() * transformation->getVelocity());
}
