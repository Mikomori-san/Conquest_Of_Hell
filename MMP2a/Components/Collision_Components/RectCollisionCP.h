//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"

class RectCollisionCP : public Component
{
public:

    RectCollisionCP(std::weak_ptr<GameObject> gameObject, std::string id, sf::Vector2f size, bool triggerBool, float scale) : Component(gameObject, id), m_colliderSize(size), m_trigger(triggerBool), m_scale(scale){}

    void update(float deltaTime) override;
    std::string getComponentId() override { return this->m_componentId; };
    void setComponentId(std::string id) override { this->m_componentId = id; }
    void init() override;
    sf::FloatRect& getCollisionRect() { return m_collisionRect; }
    void setCollisionRect(sf::FloatRect& colRect) { this->m_collisionRect = colRect; }
    void setTrigger(bool incTrigger) { this->m_trigger = incTrigger; }
    bool isTrigger() { return this->m_trigger; }

private:
    sf::FloatRect m_collisionRect;
    sf::Vector2f m_colliderSize;
    float m_scale;
    bool m_trigger;

    void updateCollider(std::shared_ptr<GameObject> go);
};