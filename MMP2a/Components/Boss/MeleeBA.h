//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "BossAbility.h"
#include "../../GameObject.h"

class MeleeBA : public BossAbility
{
public:
	MeleeBA(std::string id, std::weak_ptr<GameObject> boss, std::weak_ptr<GameObject> player) : BossAbility(id), m_playerPtr(player), m_bossPtr(boss) {};

	void init() override;
	void update(float deltaTime) override;
	void execute() override;
	bool isAttacking() { return attacking; };
private:
	void attackPlayer();
	std::weak_ptr<GameObject> m_playerPtr;
	std::weak_ptr<GameObject> m_bossPtr;
	bool attacking = false;
	int lastAnimationFrame = 0;
	const int DAMAGE = 30;
	float m_defaultAnimationSpeed = 0;
};