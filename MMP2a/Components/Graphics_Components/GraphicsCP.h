//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"
#include <memory>
#include <SFML/Graphics.hpp>

class GameObject;

class GraphicsCP : public Component 
{
public:
    GraphicsCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture)
        : Component(gameObject, id), m_sprite(std::make_shared<sf::Sprite>(sf::Sprite(texture))) 
    {}

    virtual ~GraphicsCP() = default;

    virtual std::string getComponentId() override { return m_componentId; }
    virtual void setComponentId(std::string id) override { this->m_componentId = id; }

    virtual void update(float deltaTime) override;
    virtual void init() override;
    virtual void setSprite(std::shared_ptr<sf::Texture> texture);
    virtual sf::Sprite& getSprite() { return *m_sprite; }
private:
    
protected:
    std::shared_ptr<sf::Sprite> m_sprite;
};
