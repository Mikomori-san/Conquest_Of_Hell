#pragma once
#include "AIState.h"
#include "../../../GameObject.h"

class FleePlayerState : public AIState
{
public:
	FleePlayerState(std::weak_ptr<GameObject> incGameObject, std::shared_ptr<GameObject> incPlayer) : AIState(incGameObject), player(incPlayer) {};
	void update(float deltaTime) override;
	void init() override;

private:
	std::shared_ptr<GameObject> player;
};