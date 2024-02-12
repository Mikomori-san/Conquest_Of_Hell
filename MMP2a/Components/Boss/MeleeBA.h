//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "BossAbility.h"
#include "../../GameObject.h"

class MeleeBA : public BossAbility
{
public:
	MeleeBA(std::string id, std::weak_ptr<GameObject> boss, std::weak_ptr<GameObject> player) : BossAbility(id), playerPtr(player), bossPtr(boss) {};

	void init() override;
	void update(float deltaTime) override;
	void execute() override;
	bool isAttacking() { return attacking; };
private:
	void attackPlayer();
	std::weak_ptr<GameObject> playerPtr;
	std::weak_ptr<GameObject> bossPtr;
	bool attacking = false;
	int lastAnimationFrame = 0;
	int damage = 30;
	float defaultAnimationSpeed = 0;
};