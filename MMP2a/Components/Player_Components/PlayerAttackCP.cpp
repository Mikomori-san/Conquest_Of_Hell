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
#include "../Input_Components/InputCP.h"

void PlayerAttackCP::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		auto transf = gameObject.lock()->getComponentsOfType<TransformationCP>().at(0);
		auto ani = gameObject.lock()->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0);
		auto stats = gameObject.lock()->getComponentsOfType<StatsCP>().at(0);
		auto input = gameObject.lock()->getComponentsOfType<InputCP>().at(0);

		if (InputManager::getInstance().getKeyDown(attackKey))
		{
			float distance;
			sf::Vector2f playerPos;
			sf::Vector2f enemyPos;

			if (!hasAttacked)
			{
				hasAttacked = true;
				attackTimer = 0;
				attackCooldown = 0;
				inputLocked = true;
				lastAnimation = ani->getAnimationType();
				animationLocked = true;

				if (lastAnimation == Left || lastAnimation == LeftIdle || lastAnimation == LeftAttack || lastAnimation == LeftDodge)
				{
					ani->setAnimationType(LeftAttack);
				}
				else
					ani->setAnimationType(RightAttack);

				ani->resetAnimationTimeIndex();
				originalAnimationSpeed = ani->getAnimationSpeed();
				ani->setAnimationSpeed(ani->getAnimationSpeed() * 8);

				ani->toggleAnimationLock();
				input->toggleInputLock();

				transf->setVelocity(0);
			}

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
						if ((enemyPos.x > playerPos.x && ani->getAnimationType() == RightAttack) || (enemyPos.x < playerPos.x && ani->getAnimationType() == LeftAttack))
						{
							enemy->getComponentsOfType<StatsCP>().at(0)->subtracktHealth(stats->getDamage());
						}
					}
				}
				++it;
			}
			enemies = newEnemies;
		}

		if (attackTimer > 0.2f && hasAttacked)
		{
			if (animationLocked)
			{
				ani->toggleAnimationLock();
				animationLocked = false;

				if (lastAnimation == LeftAttack || lastAnimation == LeftDodge)
				{
					ani->setAnimationType(LeftIdle);
				}
				else if (lastAnimation == RightAttack || lastAnimation == RightDodge)
				{
					ani->setAnimationType(RightIdle);
				}

				ani->setAnimationType(lastAnimation);
			}
			if (inputLocked)
			{
				input->toggleInputLock();
				inputLocked = false;
			}

			ani->setAnimationSpeed(originalAnimationSpeed);

			if (attackCooldown > 0.4f && hasAttacked)
			{
				hasAttacked = false;
			}
		}
		else if (hasAttacked)
		{
			transf->setVelocity(0);
		}

		attackTimer += deltaTime;
		attackCooldown += deltaTime;
	}
}

void PlayerAttackCP::init()
{
	hasAttacked = false;
}
