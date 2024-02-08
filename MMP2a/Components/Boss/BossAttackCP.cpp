#include "stdafx.h"
#include "BossAttackCP.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include "../../VectorAlgebra2D.h"
#include "../../Enums/Boss_Animationtype.h"
#include <iostream>

void BossAttackCP::init()
{
	ability2->init();
	charmInd->init();
}

void BossAttackCP::update(float deltaTime)
{
	timePassed += deltaTime;
	if (!playerPtr.expired() && !gameObject.expired())
	{
		std::shared_ptr<GameObject> player = playerPtr.lock();
		std::shared_ptr<GameObject> go = gameObject.lock();
		std::shared_ptr<TransformationCP> transPlayer = std::dynamic_pointer_cast<TransformationCP>(player->getComponentsOfType<TransformationCP>().at(0));
		std::shared_ptr<TransformationCP> transBoss = std::dynamic_pointer_cast<TransformationCP>(go->getComponentsOfType<TransformationCP>().at(0));
		
		sf::Vector2f playerPos = transPlayer->getPosition();
		sf::Vector2f bossPos = transBoss->getPosition();
		bossPos += transBoss->getOrigin(); //spawns ability at boss origin
		float squaredDistance = MathUtil::squaredLength(playerPos - bossPos);
		
		if (timePassed >= (attackCooldown - 0.5f) && timePassed < attackCooldown)
		{
			charmInd->setAlive();
		}

		if (attackCooldown < timePassed)
		{
			timePassed = 0;
			if (squaredDistance < swapThreshold)
			{
				executeMeele();
			}
			else
			{//doesnt function right with the time
				if (!charmIndActivated) {
					charmInd->setAlive();
					charmIndActivated = true;
				}
				if (timePassed <= (attackCooldown - 0.1f) && timePassed > attackCooldown)
				{
					charmInd->update(deltaTime); 
				}
				if (timePassed <= attackCooldown)
				{
					charmInd->setDead();
					charmIndActivated = false;
					executeCharm(bossPos, playerPos);
				}
			}
		}
	}
	ability1->update(deltaTime);
	ability2->update(deltaTime);
	charmInd->update(deltaTime);

}

void BossAttackCP::executeMeele()
{
	if (!gameObject.expired())
	{
		std::shared_ptr<MeleeBA> meele = std::dynamic_pointer_cast<MeleeBA>(ability1);
		if (meele)
		{
			meele->execute();
		}
	}
}

void BossAttackCP::executeCharm(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos)
{
	std::shared_ptr<CharmBA> charm = std::dynamic_pointer_cast<CharmBA>(ability2);
	if (charm)
	{
		if (!charm->getAlive())
		{
			charm->setAlive();
			charm->setPosition(bossPos);
			charm->setDirection(playerPos- bossPos);
		}
	}
}
