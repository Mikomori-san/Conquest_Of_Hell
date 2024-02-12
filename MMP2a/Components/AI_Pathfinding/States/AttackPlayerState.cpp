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
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		//steer to player
		go->getComponentsOfType<AStarCP>().at(0)->setTargetPos(player->getComponentsOfType<TransformationCP>().at(0)->getPosition());
		//go->getComponentsOfType<SteeringCP>().at(0)->setDestination(player->getComponentsOfType<TransformationCP>().at(0)->getPosition());
	}
}

void AttackPlayerState::init()
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
	}
}
