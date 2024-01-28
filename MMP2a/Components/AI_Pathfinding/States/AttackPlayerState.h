#pragma once
#include "AIState.h"

class AttackPlayerState : public AIState
{
public:
	AttackPlayerState(std::weak_ptr<GameObject> incGameObject, std::shared_ptr<GameObject> incPlayer) : AIState(incGameObject), player(incPlayer) {};
	void update(float deltaTime) override;
	void updateNearestPlayer(std::shared_ptr<GameObject> nearestPlayer) { player = nearestPlayer; };
	void init() override;
private:
	std::shared_ptr<GameObject> player;
};