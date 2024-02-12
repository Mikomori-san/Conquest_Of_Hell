//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Transformation_Components/BackgroundTransformationCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "StandardGraphicsCP.h"

void StandardGraphicsCP::init()
{
    sprite->setScale(sf::Vector2f(1, 1));
    sprite->setPosition(sf::Vector2f(0, 0));
}

void StandardGraphicsCP::update(float deltaTime)
{
    if (!gameObject.expired())
    {
        std::shared_ptr<TransformationCP> transform;
        std::shared_ptr<GameObject> go = gameObject.lock();

        if (go)
        {
            transform = std::dynamic_pointer_cast<TransformationCP>(go->getComponent("BackgroundTransformationCP"));
        }

        if (transform)
        {
            sprite->setPosition(transform->getPosition());
            sprite->setRotation(transform->getRotation());
            sprite->setScale(transform->getScale(), transform->getScale());
            sprite->setOrigin(transform->getOrigin());
        }
    }
}

void StandardGraphicsCP::setSprite(std::shared_ptr<sf::Texture> texture)
{
    this->sprite->setTexture(*texture);
}

sf::Vector2f StandardGraphicsCP::getSize() {
    if (sprite && sprite->getTexture()) {
        sf::Vector2u textureSize = sprite->getTexture()->getSize();
        return sf::Vector2f(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));
    }
    return sf::Vector2f(0.0f, 0.0f);
}

