#pragma once
#include "../Component.h"

class RigidBodyCP : public Component
{
public:
    RigidBodyCP(std::weak_ptr<GameObject> go, std::string id, const float newMass, const float newInvMass, sf::Vector2f vel) : Component(go, id), mass(newMass), invMass(newInvMass), velocity(vel) {};
    ~RigidBodyCP() = default;

    void init() override {};
    void update(float deltaTime) override {};

    std::string getComponentId() override { return this->componentId; }
    void setComponentId(std::string componentId) override { this->componentId = componentId; }

    void setMasses(float incMass) { this->mass = incMass; if (incMass == 0) { this->invMass = 0; } else { this->invMass = 1 / incMass; } }
    float getMass() { return this->mass; }
    float getInvMass() { return this->invMass; }

    sf::Vector2f getPos() { return this->position; }
    void setPos(sf::Vector2f pos) { this->position = pos; }
    void setPosNotifyTransf(sf::Vector2f pos);
    
    sf::Vector2f getVel() { return this->velocity; }
    void setVel(sf::Vector2f vel) { this->velocity = vel; }
    void setVelNotifyTransf(sf::Vector2f vel);

    void onCollision(std::shared_ptr<GameObject> go2);

    std::weak_ptr<GameObject> getGO() { return this->gameObject; }

private:
    float mass;
    float invMass;

    sf::Vector2f position;

    std::vector<sf::Vector2f> forces;
    std::vector<sf::Vector2f> impulses;

    sf::Vector2f acceleration;
    sf::Vector2f velocity;
};