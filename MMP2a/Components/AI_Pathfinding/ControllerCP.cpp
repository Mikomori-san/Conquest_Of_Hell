//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "States/AIState.h"
#include "ControllerCP.h"
#include "../Enemy_Components/EnemyAttackCP.h"
#include "../../GameObject.h"
#include <iostream>
#include "../StatsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "States/AttackPlayerState.h"
#include "SteeringCP.h"

void ControllerCP::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		m_health = go->getComponentsOfType<StatsCP>().at(0)->getHealth();

		float nearestDistance = std::numeric_limits<float>::max();
		sf::Vector2f myPos = go->getComponentsOfType<TransformationCP>().at(0)->getPosition();
		std::shared_ptr<GameObject> nearestPlayer;
		std::vector<std::weak_ptr<GameObject>> newPlayers;
		
		for (auto& player : m_players)
		{
			if (!player.expired())
			{
				newPlayers.push_back(player);
				auto existingPlayer = player.lock();

				sf::Vector2f playerPos = existingPlayer->getComponentsOfType<TransformationCP>().at(0)->getPosition();
				float distance = (playerPos.x - myPos.x) * (playerPos.x - myPos.x) + (playerPos.y - myPos.y) * (playerPos.y - myPos.y);

				if (nearestDistance > distance)
				{
					nearestDistance = distance;
					nearestPlayer = existingPlayer;
				}
			}
		}
		m_players = newPlayers;

		nearestDistance /= 100;

		go->getComponentsOfType<EnemyAttackCP>().at(0)->setClosestPlayer(nearestPlayer);
		
		if (!std::dynamic_pointer_cast<AttackPlayerState>(m_currentState))
		{
			m_currentState = std::make_shared<AttackPlayerState>(gameObject, nearestPlayer);
			m_currentState->init();
		}
		else if(std::shared_ptr<AttackPlayerState> aps = std::dynamic_pointer_cast<AttackPlayerState>(m_currentState))
		{
			aps->updateNearestPlayer(nearestPlayer);
		}

	}

 	m_currentState->update(deltaTime);
}

void ControllerCP::init()
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		m_maxHealth = go->getComponentsOfType<StatsCP>().at(0)->getHealth();
	}
}
