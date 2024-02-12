//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"

class RigidBodyCP : public Component
{
public:
    RigidBodyCP(std::weak_ptr<GameObject> go, std::string id, const float newMass, const float newInvMass, sf::Vector2f vel) : Component(go, id), m_mass(newMass), m_invMass(newInvMass), m_velocity(vel) {};
    ~RigidBodyCP() = default;

    void init() override {};
    void update(float deltaTime) override {};

    std::string getComponentId() override { return this->componentId; }
    void setComponentId(std::string componentId) override { this->componentId = componentId; }

    void setMasses(float incMass) { this->m_mass = incMass; if (incMass == 0) { this->m_invMass = 0; } else { this->m_invMass = 1 / incMass; } }
    float getMass() { return this->m_mass; }
    float getInvMass() { return this->m_invMass; }

    sf::Vector2f getPos() { return this->m_position; }
    void setPos(sf::Vector2f pos) { this->m_position = pos; }
    void setPosNotifyTransf(sf::Vector2f pos);
    
    sf::Vector2f getVel() { return this->m_velocity; }
    void setVel(sf::Vector2f vel) { this->m_velocity = vel; }
    void setVelNotifyTransf(sf::Vector2f vel);

    void onCollision(std::shared_ptr<GameObject> go2);

    std::weak_ptr<GameObject> getGO() { return this->gameObject; }

private:
    float m_mass;
    float m_invMass;

    sf::Vector2f m_position;

    std::vector<sf::Vector2f> m_forces;
    std::vector<sf::Vector2f> m_impulses;

    sf::Vector2f m_acceleration;
    sf::Vector2f m_velocity;
};