#pragma once
#include "../Component.h"
class RectCollisionCP : public Component
{
public:

    RectCollisionCP(std::weak_ptr<GameObject> gameObject, std::string id, sf::Vector2f size, bool triggerBool) : Component(gameObject, id), colliderSize(size), trigger(triggerBool){}

    void update(float deltaTime) override;
    std::string getComponentId() override { return this->componentId; };
    void setComponentId(std::string id) override { this->componentId = id; }
    void init() override;
    sf::FloatRect& getCollisionRect() { return collisionRect; }
    void setCollisionRect(sf::FloatRect& colRect) { this->collisionRect = colRect; }
    void setTrigger(bool incTrigger) { this->trigger = incTrigger; }
    bool isTrigger() { return this->trigger; }

private:
    sf::FloatRect collisionRect;
    sf::Vector2f colliderSize;
    bool trigger;

    void updateCollider(std::shared_ptr<GameObject> go);
};