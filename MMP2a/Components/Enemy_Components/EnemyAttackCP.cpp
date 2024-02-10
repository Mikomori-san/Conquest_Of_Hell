#include "stdafx.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include <iostream>
#include "../Transformation_Components/BackgroundTransformationCP.h"
#include "EnemyAttackCP.h"
#include "../../Enums/Enemy_Animationtype.h"
#include "../../Enums/Player_Animationtype.h"
#include "../Input_Components/InputCP.h"
#include "../StatsCP.h"

void EnemyAttackCP::doAttackAnimation(std::shared_ptr<AnimatedGraphicsCP<Enemy_Animationtype>> ani, std::shared_ptr<TransformationCP> transf)
{
	hasAttacked = true;
	attackTimer = 0;
	attackCD = 0;
	inputLocked = true;
	lastAnimation = ani->getAnimationType();
	animationLocked = true;

	if (lastAnimation == WalkLeft || lastAnimation == IdleLeft || lastAnimation == HitLeft || lastAnimation == RiseLeft)
		ani->setAnimationType(AttackLeft);
	else
		ani->setAnimationType(AttackRight);

	ani->resetAnimationTimeIndex();
	originalAnimationSpeed = ani->getAnimationSpeed();
	ani->setAnimationSpeed(ani->getAnimationSpeed() * 8);

	ani->toggleAnimationLock();

	transf->setVelocity(0);
}

void EnemyAttackCP::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		auto go = gameObject.lock();
		auto trans = go->getComponentsOfType<TransformationCP>().at(0);
		auto ani = go->getComponentsOfType<AnimatedGraphicsCP<Enemy_Animationtype>>().at(0);
		auto stats = go->getComponentsOfType<StatsCP>().at(0);
		
		if (!closestPlayer.expired())
		{
			auto cP = closestPlayer.lock();
			auto transplayer = cP->getComponentsOfType<TransformationCP>().at(0);
			auto statsPlayer = cP->getComponentsOfType<StatsCP>().at(0);

			sf::Vector2f myPos = trans->getPosition();
			sf::Vector2f playerPos = transplayer->getPosition();

			float distance = (myPos.x - playerPos.x) * (myPos.x - playerPos.x) + (myPos.y - playerPos.y) * (myPos.y - playerPos.y);
			distance /= 10;

			if (distance <= attackRange
				&& ((playerPos.x > myPos.x && ani->getAnimationType() == Enemy_Animationtype::WalkRight) || (playerPos.x < myPos.x && ani->getAnimationType() == Enemy_Animationtype::WalkLeft))
				&& attackCD > 2)
			{	
				doAttackAnimation(ani, trans);

				if (!statsPlayer->getIFrameState())
				{
					statsPlayer->subtracktHealth(stats->getDamage());
					if (statsPlayer->getHealth() <= 0)
					{
						cP->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0)->setDying();
					}
					else
					{
						cP->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0)->setColor(sf::Color::Red);
					}
					
					lastPlayerAttacked = closestPlayer;
				}
			}
		}
		if (attackTimer > 0.2f && hasAttacked)
		{
			if (animationLocked)
			{
				ani->toggleAnimationLock();
				animationLocked = false;

				if (lastAnimation == AttackLeft || lastAnimation == WalkLeft)
				{
					ani->setAnimationType(IdleLeft);
				}
				else if (lastAnimation == AttackRight || lastAnimation == WalkRight)
				{
					ani->setAnimationType(IdleRight);
				}

				ani->setAnimationType(lastAnimation);
			}
			if (inputLocked)
			{
				inputLocked = false;
			}

			ani->setAnimationSpeed(originalAnimationSpeed);

			if (attackCD > 0.4f && hasAttacked)
			{
				hasAttacked = false;
			}
		}
		else if (hasAttacked)
		{
			trans->setVelocity(0);
		}

		attackCD += deltaTime;
		attackTimer += deltaTime;
	}
}

void EnemyAttackCP::init()
{
	attackCD = 0;
}