#include "stdafx.h"
#include "PatrolState.h"
#include "../AStarCP.h"
#include "../../Transformation_Components/TransformationCP.h"

void PatrolState::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		sf::Vector2f posEnemy = go->getComponentsOfType<TransformationCP>().at(0)->getPosition();
		sf::Vector2f posPatrolPoint = patrolPoints[patrolTo]->getComponentsOfType<TransformationCP>().at(0)->getPosition();
		
		float distance = (posEnemy.x - posPatrolPoint.x) * (posEnemy.x - posPatrolPoint.x) + (posEnemy.y - posPatrolPoint.y) * (posEnemy.y - posPatrolPoint.y);
		distance /= 100;
		
		if (distance < 10)
		{
			patrolTo = patrolPoints.size() - 1 ? 0 : patrolTo + 1;
			posPatrolPoint = patrolPoints[patrolTo]->getComponentsOfType<TransformationCP>().at(0)->getPosition();
		}
		
		go->getComponentsOfType<AStarCP>().at(0)->setTargetPos(posPatrolPoint);
	}
}

void PatrolState::init()
{
	patrolTo = 0;
	
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		go->getComponentsOfType<AStarCP>().at(0)->reset();
	}
}
