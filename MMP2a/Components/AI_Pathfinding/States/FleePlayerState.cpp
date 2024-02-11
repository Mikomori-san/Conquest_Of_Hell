#include "stdafx.h"
#include "FleePlayerState.h"
#include "../AStarCP.h"

void FleePlayerState::update(float deltaTime)
{

}

void FleePlayerState::init()
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		go->getComponentsOfType<AStarCP>().at(0)->reset();
	}
}
