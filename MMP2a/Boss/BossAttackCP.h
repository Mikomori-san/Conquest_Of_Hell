#pragma once
#include "../stdafx.h"
#include "../Components/Component.h"
#include "CharmBA.h"

class BossAttackCP : public Component
{
public:
BossAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<GameObject> player) : Component(gameObject, id), playerPtr(player) {}
void init() override;
void update(float deltaTime) override;
std::string getComponentId() override { return this->componentId; }
void setComponentId(std::string id) override { componentId = id; };

//std::shared_ptr<BossAbility> const getAbility1() { return ability1; };
std::shared_ptr<BossAbility> const getAbility2() { return ability2; };

//void execute(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos);

private:
	void executeMeele();
	void executeCharm(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos);

	std::weak_ptr<GameObject> playerPtr;
	//std::shared_ptr<BossAbility> ability1 = std::make_shared<MeleeBA>(MeleeBA(gameObject, "MeleeBA"));
	std::shared_ptr<BossAbility> ability2 = std::make_shared<CharmBA>(CharmBA("CharmBA"));

	float swapThreshold = 100*100; //->squared distance
	float attackCooldown = 10;
	float timePassed = 0;
};