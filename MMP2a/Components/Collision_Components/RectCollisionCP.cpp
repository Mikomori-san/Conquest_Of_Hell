//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "RectCollisionCP.h"

void RectCollisionCP::update(float deltaTime)
{
    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();
        updateCollider(go);

        std::shared_ptr<TransformationCP> transf = go->getComponentsOfType<TransformationCP>().at(0);

        std::shared_ptr<GraphicsCP> ani;

        if (go->getComponentsOfType<GraphicsCP>().size() != 0)
        {
                ani = go->getComponentsOfType<GraphicsCP>().at(0);
                m_collisionRect = sf::FloatRect(
                    transf->getPosition().x,
                    transf->getPosition().y,
                    ani->getSprite().getGlobalBounds().width * m_scale,
                    ani->getSprite().getGlobalBounds().height * m_scale
                );
        }
        else
        {
            m_collisionRect = sf::FloatRect(
                transf->getPosition().x,
                transf->getPosition().y,
                m_colliderSize.x * m_scale,
                m_colliderSize.y * m_scale
            );
        }
    }
}

void RectCollisionCP::init()
{
    if (!gameObject.expired())
    {
        std::shared_ptr<GameObject> go = gameObject.lock();
        updateCollider(go);
    }
}

void RectCollisionCP::updateCollider(std::shared_ptr<GameObject> go)
{
    std::shared_ptr<TransformationCP> col = go->getComponentsOfType<TransformationCP>().at(0);

    if (go->getComponentsOfType<GraphicsCP>().size() == 0)
    {
        m_collisionRect = sf::FloatRect(
            col->getPosition().x - m_colliderSize.x / 2,
            col->getPosition().y - m_colliderSize.y / 2,
            m_colliderSize.x,
            m_colliderSize.y
        );
    }
    else
    {
        std::shared_ptr<GraphicsCP> ani = go->getComponentsOfType<GraphicsCP>().at(0);
        m_collisionRect = sf::FloatRect(
            col->getPosition().x - m_colliderSize.x / 2,
            col->getPosition().y - m_colliderSize.y / 2,
            ani->getSprite().getGlobalBounds().width,
            ani->getSprite().getGlobalBounds().height
        );
    }
}