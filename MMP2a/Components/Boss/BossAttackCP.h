#pragma once
#include "../../Components/Component.h"
#include "CharmBA.h"
#include  "MeleeBA.h"
#include  "CharmIndicator.h"
#include  "MeeleeIndicator.h"

#include "../../Enums/Boss_Animationtype.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"

class BossAttackCP : public Component
{
public:
BossAttackCP(std::shared_ptr<Component> bossGraphicsCP, std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<GameObject> player) : Component(gameObject, id), m_bossGraphicsCP(bossGraphicsCP), playerPtr(player) {}
void init() override;
void update(float deltaTime) override;
std::string getComponentId() override { return this->componentId; }
void setComponentId(std::string id) override { componentId = id; };

//std::shared_ptr<BossAbility> const getAbility1() { return ability1; };
std::shared_ptr<BossAbility> const getAbility2() { return ability2; };
std::shared_ptr<CharmIndicator> const getCharmInd() { return charmInd; };
std::shared_ptr<MeeleeIndicator> const getMeeleeInd() { return meeleeInd; };

//void execute(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos);

private:
	void executeMeele();
	void executeCharm(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos);

	std::weak_ptr<GameObject> playerPtr;
	std::shared_ptr<BossAbility> ability1 = std::make_shared<MeleeBA>(MeleeBA("MeleeBA", gameObject, playerPtr));
	std::shared_ptr<BossAbility> ability2 = std::make_shared<CharmBA>(CharmBA("CharmBA", playerPtr));
	std::shared_ptr<CharmIndicator> charmInd = std::make_shared<CharmIndicator>(CharmIndicator());
	std::shared_ptr<MeeleeIndicator> meeleeInd = std::make_shared<MeeleeIndicator>(MeeleeIndicator());
	std::shared_ptr<Component> m_bossGraphicsCP;


	float swapThreshold = 100.f * 100.f; //->squared distance
	float attackCooldown = 2.f;
	float timePassed = 0.f;

};