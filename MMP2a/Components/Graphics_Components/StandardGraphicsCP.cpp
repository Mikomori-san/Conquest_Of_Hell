//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Transformation_Components/BackgroundTransformationCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "StandardGraphicsCP.h"

void StandardGraphicsCP::init()
{
    m_sprite->setScale(sf::Vector2f(1, 1));
    m_sprite->setPosition(sf::Vector2f(0, 0));
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
            m_sprite->setPosition(transform->getPosition());
            m_sprite->setRotation(transform->getRotation());
            m_sprite->setScale(transform->getScale(), transform->getScale());
            m_sprite->setOrigin(transform->getOrigin());
        }
    }
}

void StandardGraphicsCP::setSprite(std::shared_ptr<sf::Texture> texture)
{
    this->m_sprite->setTexture(*texture);
}

sf::Vector2f StandardGraphicsCP::getSize() {
    if (m_sprite && m_sprite->getTexture()) {
        sf::Vector2u textureSize = m_sprite->getTexture()->getSize();
        return sf::Vector2f(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));
    }
    return sf::Vector2f(0.0f, 0.0f);
}

