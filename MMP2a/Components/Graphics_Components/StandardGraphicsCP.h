#pragma once
#include "GraphicsCP.h"

class StandardGraphicsCP : public GraphicsCP {
public:
    StandardGraphicsCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture)
        : GraphicsCP(gameObject, id, texture)
    {}

    ~StandardGraphicsCP() = default;

    std::string getComponentId() override { return componentId; }
    void setComponentId(std::string id) override { this->componentId = id; }
    void init() override;
    void update(float deltaTime) override;
    void setSprite(std::shared_ptr<sf::Texture> texture) override;
    sf::Vector2f getSize();
    sf::Sprite& getSprite() override { return *sprite; }
};
