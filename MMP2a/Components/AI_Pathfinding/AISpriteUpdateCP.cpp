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
        float speed = trans->getVelocity();

        if (dir.x > 0 && dir.x < 1 && dir.y < 0 && dir.y > -1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::RightIdle);
                    lastAnimation = Player_Animationtype::RightIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Right);
                    lastAnimation = Player_Animationtype::Right;
                }
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::RightIdle);
                    lastAnimation = Player_Animationtype::RightIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Right);
                    lastAnimation = Player_Animationtype::Right;
                }
            }
        }
        else if (dir.x > 0 && dir.x < 1 && dir.y > 0 && dir.y < 1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::RightIdle);
                    lastAnimation = Player_Animationtype::RightIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Right);
                    lastAnimation = Player_Animationtype::Right;
                }
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::RightIdle);
                    lastAnimation = Player_Animationtype::RightIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Right);
                    lastAnimation = Player_Animationtype::Right;
                }
            }
        }
        else if (dir.x < 0 && dir.x > -1 && dir.y > 0 && dir.y < 1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::LeftIdle);
                    lastAnimation = Player_Animationtype::LeftIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Left);
                    lastAnimation = Player_Animationtype::Left;
                }
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::LeftIdle);
                    lastAnimation = Player_Animationtype::LeftIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Left);
                    lastAnimation = Player_Animationtype::Left;
                }
            }
        }
        else if (dir.x < 0 && dir.x > -1 && dir.y < 0 && dir.y > -1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::LeftIdle);
                    lastAnimation = Player_Animationtype::LeftIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Left);
                    lastAnimation = Player_Animationtype::Left;
                }

            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Player_Animationtype::LeftIdle);
                    lastAnimation = Player_Animationtype::LeftIdle;
                }
                else
                {
                    ani->setAnimationType(Player_Animationtype::Left);
                    lastAnimation = Player_Animationtype::Left;
                }
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
