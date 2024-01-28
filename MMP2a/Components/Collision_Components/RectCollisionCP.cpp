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
            ani = go->getComponentsOfType<GraphicsCP>().at(0);
            collisionRect = sf::FloatRect(
                transf->getPosition().x,
                transf->getPosition().y,
                ani->getSprite().getGlobalBounds().width,
                ani->getSprite().getGlobalBounds().height
            );
        }
        else
        {
            collisionRect = sf::FloatRect(
                transf->getPosition().x,
                transf->getPosition().y,
                colliderSize.x,
                colliderSize.y
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