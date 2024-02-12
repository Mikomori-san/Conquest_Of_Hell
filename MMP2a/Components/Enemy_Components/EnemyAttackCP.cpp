//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Input_Components/InputCP.h"
#include "../StatsCP.h"
//#include "../Transformation_Components/BackgroundTransformationCP.h"
#include "EnemyAttackCP.h"
#include "../../Enums/Enemy_Animationtype.h"
#include "../../Enums/Player_Animationtype.h"
#include <iostream>

void EnemyAttackCP::doAttackAnimation(std::shared_ptr<AnimatedGraphicsCP<Enemy_Animationtype>> ani, std::shared_ptr<TransformationCP> transf)
{
	m_hasAttacked = true;
	m_attackTimer = 0;
	m_attackCD = 0;
	m_inputLocked = true;
	m_lastAnimation = ani->getAnimationType();
	m_animationLocked = true;

	if (m_lastAnimation == WalkLeft || m_lastAnimation == IdleLeft || m_lastAnimation == HitLeft || m_lastAnimation == RiseLeft)
		ani->setAnimationType(AttackLeft);
	else
		ani->setAnimationType(AttackRight);

	ani->resetAnimationTimeIndex();
	m_originalAnimationSpeed = ani->getAnimationSpeed();
	ani->setAnimationSpeed(ani->getAnimationSpeed() * 8);

	ani->toggleAnimationLock();

	transf->setVelocity(0);
}

void EnemyAttackCP::update(float deltaTime)
{
	if (!m_gameObject.expired())
	{
		auto go = m_gameObject.lock();
		auto trans = go->getComponentsOfType<TransformationCP>().at(0);
		auto ani = go->getComponentsOfType<AnimatedGraphicsCP<Enemy_Animationtype>>().at(0);
		auto stats = go->getComponentsOfType<StatsCP>().at(0);
		
		if (!m_closestPlayer.expired())
		{
			auto cP = m_closestPlayer.lock();
			auto transplayer = cP->getComponentsOfType<TransformationCP>().at(0);
			auto statsPlayer = cP->getComponentsOfType<StatsCP>().at(0);

			sf::Vector2f myPos = trans->getPosition();
			sf::Vector2f playerPos = transplayer->getPosition();

			float distance = (myPos.x - playerPos.x) * (myPos.x - playerPos.x) + (myPos.y - playerPos.y) * (myPos.y - playerPos.y);
			distance /= 10;

			if (distance <= m_attackRange
				&& ((playerPos.x > myPos.x && ani->getAnimationType() == Enemy_Animationtype::WalkRight) || (playerPos.x < myPos.x && ani->getAnimationType() == Enemy_Animationtype::WalkLeft))
				&& m_attackCD > 2)
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
					
					m_lastPlayerAttacked = m_closestPlayer;
				}
			}
		}
		if (m_attackTimer > 0.2f && m_hasAttacked)
		{
			if (m_animationLocked)
			{
				ani->toggleAnimationLock();
				m_animationLocked = false;

				if (m_lastAnimation == AttackLeft || m_lastAnimation == WalkLeft)
				{
					ani->setAnimationType(IdleLeft);
				}
				else if (m_lastAnimation == AttackRight || m_lastAnimation == WalkRight)
				{
					ani->setAnimationType(IdleRight);
				}

				ani->setAnimationType(m_lastAnimation);
			}
			if (m_inputLocked)
			{
				m_inputLocked = false;
			}

			ani->setAnimationSpeed(m_originalAnimationSpeed);

			if (m_attackCD > 0.4f && m_hasAttacked)
			{
				m_hasAttacked = false;
			}
		}
		else if (m_hasAttacked)
		{
			trans->setVelocity(0);
		}

		m_attackCD += deltaTime;
		m_attackTimer += deltaTime;
	}
}

void EnemyAttackCP::init()
{
	m_attackCD = 0;
}