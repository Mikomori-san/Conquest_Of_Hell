#include "stdafx.h"
#include "RectCollisionCP.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Transformation_Components/TransformationCP.h"

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
            if (!Component::componentId.find("BossAttackRange"))
            {
                ani = go->getComponentsOfType<GraphicsCP>().at(0);
                collisionRect = sf::FloatRect(
                    transf->getPosition().x - ani->getSprite().getGlobalBounds().width,
                    transf->getPosition().y - ani->getSprite().getGlobalBounds().height,
                    ani->getSprite().getGlobalBounds().width * 3,
                    ani->getSprite().getGlobalBounds().height * 3
                );
            }
            else
            {
                ani = go->getComponentsOfType<GraphicsCP>().at(0);
                collisionRect = sf::FloatRect(
                    transf->getPosition().x,
                    transf->getPosition().y,
                    ani->getSprite().getGlobalBounds().width * scale,
                    ani->getSprite().getGlobalBounds().height * scale
                );
            }
        }
        else
        {
            collisionRect = sf::FloatRect(
                transf->getPosition().x,
                transf->getPosition().y,
                colliderSize.x * scale,
                colliderSize.y * scale
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
        collisionRect = sf::FloatRect(
            col->getPosition().x - colliderSize.x / 2,
            col->getPosition().y - colliderSize.y / 2,
            colliderSize.x,
            colliderSize.y
        );
    }
    else
    {
        std::shared_ptr<GraphicsCP> ani = go->getComponentsOfType<GraphicsCP>().at(0);
        collisionRect = sf::FloatRect(
            col->getPosition().x - colliderSize.x / 2,
            col->getPosition().y - colliderSize.y / 2,
            ani->getSprite().getGlobalBounds().width,
            ani->getSprite().getGlobalBounds().height
        );
    }
}