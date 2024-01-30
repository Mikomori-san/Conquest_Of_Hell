#include "stdafx.h"
#include "BossAttackCP.h"
#include "../Components/Transformation_Components/TransformationCP.h"
#include "../VectorAlgebra2D.h"

void BossAttackCP::init()
{
	ability2->init();
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

		float squaredDistance = MathUtil::squaredLength(playerPos - bossPos);
		if (squaredDistance < swapThreshold)
		{
			executeMeele();
		}
		else
		{
			executeCharm(bossPos, playerPos);
		}
	}
	//ability2->update(deltaTime);

	
	//ability1->update(deltaTime);
}

void BossAttackCP::executeMeele()
{

}

void BossAttackCP::executeCharm(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos)
{
	std::shared_ptr<CharmBA> charm = std::dynamic_pointer_cast<CharmBA>(ability2);
	if (charm)
	{
		if (!charm->getAlive() && attackCooldown < timePassed)
		{
			timePassed = 0;
			charm->setAlive();
			charm->setPosition(bossPos);
			charm->setDirection(playerPos- bossPos);
		}
	}
}
