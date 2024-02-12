//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../Components/Component.h"
#include "../../Enums/Boss_Animationtype.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "CharmBA.h"
#include "CharmIndicator.h"
#include "MeleeBA.h"
#include "MeleeIndicator.h"

class BossAttackCP : public Component
{
public:
	BossAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<GameObject> player) : Component(gameObject, id), playerPtr(player) {}
	void init() override;
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->componentId; }
	void setComponentId(std::string id) override { componentId = id; };

	std::shared_ptr<BossAbility> const getAbility1() { return ability1; };
	std::shared_ptr<BossAbility> const getAbility2() { return ability2; };
	std::shared_ptr<CharmIndicator> const getCharmInd() { return charmInd; };
	std::shared_ptr<MeleeIndicator> const getMeeleeInd() { return meeleeInd; };


private:
	void executeMeele();
	void executeCharm(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos);

	std::weak_ptr<GameObject> playerPtr;
	std::shared_ptr<BossAbility> ability1 = std::make_shared<MeleeBA>(MeleeBA("MeleeBA", gameObject, playerPtr));
	std::shared_ptr<BossAbility> ability2 = std::make_shared<CharmBA>(CharmBA("CharmBA", playerPtr));
	std::shared_ptr<CharmIndicator> charmInd = std::make_shared<CharmIndicator>(CharmIndicator(gameObject));
	std::shared_ptr<MeleeIndicator> meeleeInd = std::make_shared<MeleeIndicator>(MeleeIndicator(gameObject));

	float swapThreshold = 150.f * 150.f; //->squared distance
	float attackCooldown = 5.f;
	float timePassed = 0.f;

};