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
	BossAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<GameObject> player) : Component(gameObject, id), m_playerPtr(player) {}
	void init() override;
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->m_componentId; }
	void setComponentId(std::string id) override { m_componentId = id; };

	std::shared_ptr<BossAbility> const getAbility1() { return m_ability1; };
	std::shared_ptr<BossAbility> const getAbility2() { return m_ability2; };
	std::shared_ptr<CharmIndicator> const getCharmInd() { return m_charmInd; };
	std::shared_ptr<MeleeIndicator> const getMeeleeInd() { return m_meleeInd; };


private:
	void executeMeele();
	void executeCharm(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos);

	std::weak_ptr<GameObject> m_playerPtr;
	std::shared_ptr<BossAbility> m_ability1 = std::make_shared<MeleeBA>(MeleeBA("MeleeBA", m_gameObject, m_playerPtr));
	std::shared_ptr<BossAbility> m_ability2 = std::make_shared<CharmBA>(CharmBA("CharmBA", m_playerPtr));
	std::shared_ptr<CharmIndicator> m_charmInd = std::make_shared<CharmIndicator>(CharmIndicator(m_gameObject));
	std::shared_ptr<MeleeIndicator> m_meleeInd = std::make_shared<MeleeIndicator>(MeleeIndicator(m_gameObject));

	const float SWAP_THRESHOLD = 150.f * 150.f; //->squared distance
	const float ATTACK_COOLDOWN = 2.5f;
	float timePassed = 0.f;
};