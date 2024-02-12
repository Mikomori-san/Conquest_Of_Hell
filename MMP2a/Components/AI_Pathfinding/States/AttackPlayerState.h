//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "AIState.h"

class AttackPlayerState : public AIState
{
public:
	AttackPlayerState(std::weak_ptr<GameObject> incGameObject, std::shared_ptr<GameObject> incPlayer) : AIState(incGameObject), m_player(incPlayer) {};
	void update(float deltaTime) override;
	void updateNearestPlayer(std::shared_ptr<GameObject> nearestPlayer) { m_player = nearestPlayer; };
	void init() override;
private:
	std::shared_ptr<GameObject> m_player;
};