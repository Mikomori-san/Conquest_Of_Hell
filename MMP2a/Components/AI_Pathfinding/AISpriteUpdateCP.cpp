//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../../Enums/Enemy_Animationtype.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "AISpriteUpdateCP.h"

void AISpriteUpdateCP::update(float deltaTime)
{
    if (!m_gameObject.expired())
    {
        std::shared_ptr<GameObject> go = m_gameObject.lock();
        std::shared_ptr<AnimatedGraphicsCP<Enemy_Animationtype>> ani = go->getComponentsOfType<AnimatedGraphicsCP<Enemy_Animationtype>>().at(0);
        std::shared_ptr<TransformationCP> trans = go->getComponentsOfType<TransformationCP>().at(0);

        sf::Vector2f dir = trans->getDirection();
        float speed = trans->getVelocity();

        if (dir.x > 0 && dir.x < 1 && dir.y < 0 && dir.y > -1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleRight);
                    m_lastAnimation = Enemy_Animationtype::IdleRight;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkRight);
                    m_lastAnimation = Enemy_Animationtype::WalkRight;
                }
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(m_lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleRight);
                    m_lastAnimation = Enemy_Animationtype::IdleRight;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkRight);
                    m_lastAnimation = Enemy_Animationtype::WalkRight;
                }
            }
        }
        else if (dir.x > 0 && dir.x < 1 && dir.y > 0 && dir.y < 1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleRight);
                    m_lastAnimation = Enemy_Animationtype::IdleRight;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkRight);
                    m_lastAnimation = Enemy_Animationtype::WalkRight;
                }
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(m_lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleRight);
                    m_lastAnimation = Enemy_Animationtype::IdleRight;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkRight);
                    m_lastAnimation = Enemy_Animationtype::WalkRight;
                }
            }
        }
        else if (dir.x < 0 && dir.x > -1 && dir.y > 0 && dir.y < 1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleLeft);
                    m_lastAnimation = Enemy_Animationtype::IdleLeft;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkLeft);
                    m_lastAnimation = Enemy_Animationtype::WalkLeft;
                }
            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(m_lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleLeft);
                    m_lastAnimation = Enemy_Animationtype::IdleLeft;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkLeft);
                    m_lastAnimation = Enemy_Animationtype::WalkLeft;
                }
            }
        }
        else if (dir.x < 0 && dir.x > -1 && dir.y < 0 && dir.y > -1)
        {
            if (abs(dir.x) > abs(dir.y) && abs(dir.y) < 0.25f)
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleLeft);
                    m_lastAnimation = Enemy_Animationtype::IdleLeft;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkLeft);
                    m_lastAnimation = Enemy_Animationtype::WalkLeft;
                }

            }
            else if (abs(dir.x) < abs(dir.y) && abs(dir.x) < 0.25f)
            {
                ani->setAnimationType(m_lastAnimation);
            }
            else
            {
                if (speed == 0)
                {
                    ani->setAnimationType(Enemy_Animationtype::IdleLeft);
                    m_lastAnimation = Enemy_Animationtype::IdleLeft;
                }
                else
                {
                    ani->setAnimationType(Enemy_Animationtype::WalkLeft);
                    m_lastAnimation = Enemy_Animationtype::WalkLeft;
                }
            }
        }
    }
}

std::string AISpriteUpdateCP::getComponentId()
{
    return this->m_componentId;
}

void AISpriteUpdateCP::setComponentId(std::string id)
{
    this->m_componentId = id;
}

void AISpriteUpdateCP::init()
{
}
