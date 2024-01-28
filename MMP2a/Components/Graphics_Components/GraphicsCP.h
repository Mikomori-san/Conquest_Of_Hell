#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../Component.h"

class GameObject;

class GraphicsCP : public Component 
{
public:
    GraphicsCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture)
        : Component(gameObject, id), sprite(std::make_shared<sf::Sprite>(sf::Sprite(texture))) 
    {}

    virtual ~GraphicsCP() = default;

    virtual std::string getComponentId() override { return componentId; }
    virtual void setComponentId(std::string id) override { this->componentId = id; }

    virtual void update(float deltaTime) override;
    virtual void init() override;
    virtual void setSprite(std::shared_ptr<sf::Texture> texture);
    virtual sf::Sprite& getSprite() { return *sprite; }
private:
    
protected:
    std::shared_ptr<sf::Sprite> sprite;
};
