//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "BossAttackCP.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include "../../Math/VectorAlgebra2D.h"
#include "../../Enums/Boss_Animationtype.h"
#include <iostream>

void BossAttackCP::init()
{
	m_ability2->init();
	m_charmInd->init();
	m_meleeInd->init();
}

void BossAttackCP::update(float deltaTime)
{
	timePassed += deltaTime;
	if (!m_playerPtr.expired() && !m_gameObject.expired())
	{
		std::shared_ptr<GameObject> player = m_playerPtr.lock();
		std::shared_ptr<GameObject> go = m_gameObject.lock();
		std::shared_ptr<TransformationCP> transPlayer = std::dynamic_pointer_cast<TransformationCP>(player->getComponentsOfType<TransformationCP>().at(0));
		std::shared_ptr<TransformationCP> transBoss = std::dynamic_pointer_cast<TransformationCP>(go->getComponentsOfType<TransformationCP>().at(0));
		
		sf::Vector2f playerPos = transPlayer->getPosition();
		sf::Vector2f bossPos = transBoss->getPosition();
		bossPos += transBoss->getOrigin(); //spawns ability at boss origin
		float squaredDistance = MathUtil::squaredLength(playerPos - bossPos);
		

		if (ATTACK_COOLDOWN < timePassed)
		{
			m_charmInd->setDead();
			m_meleeInd->setDead();
			timePassed = 0;
			if (squaredDistance < SWAP_THRESHOLD)
			{
				executeMeele();
			}
			else
			{				
				executeCharm(bossPos, playerPos);
				m_charmInd->setAlive();
			}
		}
	}

	m_ability1->update(deltaTime);
	m_ability2->update(deltaTime);
	m_charmInd->update(deltaTime);
	m_meleeInd->update(deltaTime);

}

void BossAttackCP::executeMeele()
{
	if (!m_gameObject.expired())
	{
		std::shared_ptr<MeleeBA> meele = std::dynamic_pointer_cast<MeleeBA>(m_ability1);
		if (meele)
		{
			m_meleeInd->setAlive();
			meele->execute();
		}
	}
}

void BossAttackCP::executeCharm(const sf::Vector2f& bossPos, const sf::Vector2f& playerPos)
{
	std::shared_ptr<CharmBA> charm = std::dynamic_pointer_cast<CharmBA>(m_ability2);
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
