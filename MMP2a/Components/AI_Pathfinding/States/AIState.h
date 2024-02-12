//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../../GameObject.h"
class AIState
{
public:
	AIState(std::weak_ptr<GameObject> incGameObject) : m_gameObject(incGameObject) {};
	virtual void update(float deltaTime) = 0;
	virtual void init() = 0;

protected:
	std::weak_ptr<GameObject> m_gameObject;
};