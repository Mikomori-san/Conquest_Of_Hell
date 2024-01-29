#pragma once
#include "../Component.h"

class PlayerAttackCP : public Component
{
public:
    PlayerAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, int incAR, std::vector<std::weak_ptr<GameObject>> incEnemies, sf::Keyboard::Key incKey) : Component(gameObject, id), enemies(incEnemies), attackKey(incKey), attackRange(incAR) {}

    void update(float deltaTime) override;
    std::string getComponentId() override { return this->componentId; };
    void setComponentId(std::string id) override { this->componentId = id; }
    void init() override;
    void addEnemy(std::weak_ptr<GameObject> enemy) { enemies.push_back(enemy); }
private:
    std::vector<std::weak_ptr<GameObject>> enemies;
    sf::Keyboard::Key attackKey;
    int attackRange;
};