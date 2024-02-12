//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "AttackPlayerState.h"
#include "../AStarCP.h"
#include "../../Transformation_Components/TransformationCP.h"
#include "../SteeringCP.h"

void AttackPlayerState::update(float deltaTime)
{
	if (!m_gameObject.expired())
	{
		std::shared_ptr<GameObject> go = m_gameObject.lock();
		//steer to player
		go->getComponentsOfType<AStarCP>().at(0)->setTargetPos(m_player->getComponentsOfType<TransformationCP>().at(0)->getPosition());
		//go->getComponentsOfType<SteeringCP>().at(0)->setDestination(player->getComponentsOfType<TransformationCP>().at(0)->getPosition());
	}
}

void AttackPlayerState::init()
{
	if (!m_gameObject.expired())
	{
		std::shared_ptr<GameObject> go = m_gameObject.lock();
	}
}
