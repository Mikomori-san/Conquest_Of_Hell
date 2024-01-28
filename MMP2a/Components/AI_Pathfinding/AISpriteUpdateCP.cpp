#include "stdafx.h"
#include "AISpriteUpdateCP.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Transformation_Components/TransformationCP.h"

void AISpriteUpdateCP::update(float deltaTime)
{
    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();
        std::shared_ptr<AnimatedGraphicsCP> ani = go->getComponentsOfType<AnimatedGraphicsCP>().at(0);
        std::shared_ptr<TransformationCP> trans = go->getComponentsOfType<TransformationCP>().at(0);

        sf::Vector2f dir = trans->getDirection();

        if (dir.x > 0 && dir.x < 1 && dir.y < 0 && dir.y > -1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Right);
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Up);
            }
            else
            {
                ani->setAnimationType(Animationtype::UpRight);
            }
        }
        else if (dir.x > 0 && dir.x < 1 && dir.y > 0 && dir.y < 1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Right);
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Down);
            }
            else
            {
                ani->setAnimationType(Animationtype::RightDown);
            }
        }
        else if (dir.x < 0 && dir.x > -1 && dir.y > 0 && dir.y < 1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Left);
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Down);
            }
            else
            {
                ani->setAnimationType(Animationtype::DownLeft);
            }
        }
        else if (dir.x < 0 && dir.x > -1 && dir.y < 0 && dir.y > -1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Left);
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(Animationtype::Up);
            }
            else
            {
                ani->setAnimationType(Animationtype::LeftUp);
            }
        }
    }
}

std::string AISpriteUpdateCP::getComponentId()
{
    return this->componentId;
}

void AISpriteUpdateCP::setComponentId(std::string id)
{
    this->componentId = id;
}

void AISpriteUpdateCP::init()
{
}
