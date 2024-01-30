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
	//ability1->update(deltaTime);
	ability2->update(deltaTime);
}

void BossAttackCP::execute(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos)
{
	float squaredDistance = MathUtil::squaredLength(playerPos- bossPos);
	if (squaredDistance < swapThreshold)
	{
		executeMeele();
	}
	else
	{
		executeCharm(bossPos, playerPos);
	}
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
			charm->execute();
		}
	}
}
