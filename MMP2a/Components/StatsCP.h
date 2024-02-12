//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "Component.h"

class StatsCP : public Component
{
public:

    StatsCP(std::weak_ptr<GameObject> gameObject, std::string id, int hp, int dmg, std::string incObjType) : Component(gameObject, id), m_maxHealth(hp), m_gameObjectType(incObjType), m_originalDamage(dmg) {};

    void update(float deltaTime) override {};

    std::string getComponentId() override
    {
        return this->m_componentId;
    }

    void setComponentId(std::string id) override
    {
        this->m_componentId = id;
    }

    void init() override;

    void subtracktHealth(int dmg) { m_health -= dmg; std::cout << m_health << std::endl; }
    void addHealth(int healing) { m_health += healing; }
    int getHealth() { return m_health; }
    void setHealth(int newHealth) { m_health = newHealth; }

    int getMaxHealth() { return m_maxHealth; }

    int getDamage() { return m_damage; }

    void toggleIFrames() { m_hasIFrames = m_hasIFrames ? false : true; }
    bool getIFrameState() { return m_hasIFrames; }

    std::string getObjectType() { return this->m_gameObjectType; }

    void setDeath() { m_isDead = true; }
    bool hasDied() { return m_isDead; }

private:
    int m_maxHealth;
    int m_health;
    int m_originalDamage;
    int m_damage;
    std::string m_gameObjectType;
    bool m_hasIFrames = false;
    bool m_isDead;
};