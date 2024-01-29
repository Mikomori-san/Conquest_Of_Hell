#include <iostream>
#include "../StatsCP.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Transformation_Components/BackgroundTransformationCP.h"
#include "stdafx.h"
#include "PlayerAttackCP.h"
#include "../../Manager/InputManager.h"
#include "../Transformation_Components/TransformationCP.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../StatsCP.h"

void PlayerAttackCP::update(float deltaTime)
{
	if (InputManager::getInstance().getKeyDown(sf::Keyboard::Q))
	{
		float distance;
		sf::Vector2f playerPos;
		sf::Vector2f enemyPos;

		if (!gameObject.expired())
		{
			std::shared_ptr<TransformationCP> transf = gameObject.lock()->getComponentsOfType<TransformationCP>().at(0);
			std::shared_ptr<AnimatedGraphicsCP> ani = gameObject.lock()->getComponentsOfType<AnimatedGraphicsCP>().at(0);
			std::shared_ptr<StatsCP> stats = gameObject.lock()->getComponentsOfType<StatsCP>().at(0);
																																//ANIMATION ZU ATTACK
			playerPos = transf->getPosition();
			std::vector<std::weak_ptr<GameObject>> newEnemies;

			for (auto it = enemies.begin(); it != enemies.end();)
			{
				if (!it->expired())
				{
					newEnemies.push_back(*it);

					auto enemy = it->lock();
					enemyPos = enemy->getComponentsOfType<TransformationCP>().at(0)->getPosition();
					distance = (playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + (playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y);
					distance /= 10;
					std::cout << "Distance: " << distance << " Range: " << attackRange << std::endl;
					if (distance <= attackRange)
					{
						if (enemyPos.x > playerPos.x && ani->getAnimationType() == Right || ani->getAnimationType() == RightIdle)
						{
							// Enemy ist auf rechter Seite & Player faced rechts
							// HIT
							std::cout << "HIT" << std::endl;
							enemy->getComponentsOfType<StatsCP>().at(0)->subtracktHealth(stats->getDamage());
						}
						else if (enemyPos.x < playerPos.x && ani->getAnimationType() == Left || ani->getAnimationType() == LeftIdle)
						{
							// Enemy ist auf linker Seite
							// HIT
							std::cout << "HIT" << std::endl;
							enemy->getComponentsOfType<StatsCP>().at(0)->subtracktHealth(stats->getDamage());
						}
					}
				}
				++it;
			}
			enemies = newEnemies;
		}
	}
}

void PlayerAttackCP::init()
{

}