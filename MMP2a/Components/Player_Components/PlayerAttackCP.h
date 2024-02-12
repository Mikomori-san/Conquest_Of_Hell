//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../Enums/GamepadButton.h"
#include "../../Manager/AssetManager.h"
#include "../Component.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Input_Components/InputCP.h"
#include "../Input_Components/MovementInputGamepadCP.h"
#include "../Player_Components/DashCP.h"
#include "../StatsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "../../Manager/InputManager.h"
#include "../Boss/BossAttackCP.h"
#include "../Boss/MeleeBA.h"

template <typename T>
class PlayerAttackCP : public Component
{
public:
    PlayerAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, int incAR, std::vector<std::weak_ptr<GameObject>> incEnemies, T incKey) : Component(gameObject, id), m_enemies(incEnemies), m_attackKey(incKey), m_attackRange(incAR) {}

    void update(float deltaTime) override;
    std::string getComponentId() override { return this->componentId; };
    void setComponentId(std::string id) override { this->componentId = id; }
    void init() override;
    void addEnemy(std::weak_ptr<GameObject> enemy) { m_enemies.push_back(enemy); }
	void setAttackLock(bool al) { m_attackLock = al; }
	void setBoss(std::weak_ptr<GameObject> incBoss) { m_boss = incBoss; }
private:
    std::vector<std::weak_ptr<GameObject>> m_enemies;
    T m_attackKey;
    int m_attackRange;

	std::weak_ptr<GameObject> m_boss;

    bool m_hasAttacked;
    bool m_inputLocked;
    bool m_animationLocked;

    float m_attackTimer;
    float m_attackCooldown;

    int m_originalAnimationSpeed;
    Player_Animationtype m_lastAnimation;

    sf::Color m_lastColor;

    void doAttack(std::shared_ptr<TransformationCP> trans, std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani,
        std::shared_ptr<StatsCP> stats, std::shared_ptr<InputCP> input);

	bool m_attackLock = false;
	bool m_useController = false;
	sf::Music m_swordHitSound;
};


template<typename T>
void PlayerAttackCP<T>::init()
{
	m_hasAttacked = false;
	AssetManager::getInstance().Music["Player_Hit"]->setVolume(6.f);
	AssetManager::getInstance().Music["Bones"]->setVolume(10);
}

template<typename T>
void PlayerAttackCP<T>::doAttack(std::shared_ptr<TransformationCP> transf, std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani,
	std::shared_ptr<StatsCP> stats, std::shared_ptr<InputCP> input)
{
	AssetManager::getInstance().Music["Player_Hit"]->play();

	float distance;
	sf::Vector2f playerPos;
	sf::Vector2f enemyPos;

	m_hasAttacked = true;
	m_attackTimer = 0;
	m_attackCooldown = 0;
	m_inputLocked = true;
	m_lastAnimation = ani->getAnimationType();
	m_animationLocked = true;

	if (!ani->isAnimationLock())
	{
		if (m_lastAnimation == Left || m_lastAnimation == LeftIdle || m_lastAnimation == LeftAttack || m_lastAnimation == LeftDodge)
		{
			ani->setAnimationType(LeftAttack);
		}
		else
			ani->setAnimationType(RightAttack);

		ani->resetAnimationTimeIndex();
		m_originalAnimationSpeed = ani->getAnimationSpeed();
		ani->setAnimationSpeed(ani->getAnimationSpeed() * 8);

		ani->toggleAnimationLock();
	}
	input->toggleInputLock();

	transf->setVelocity(0);

	playerPos = transf->getPosition();
	std::vector<std::weak_ptr<GameObject>> newEnemies;

	if (!m_boss.expired())
	{
		auto daBoss = m_boss.lock();
		auto bossPos = daBoss->getComponentsOfType<TransformationCP>().at(0)->getPosition();
		std::shared_ptr<MeleeBA> abilityPtr = std::dynamic_pointer_cast<MeleeBA>(daBoss->getComponentsOfType<BossAttackCP>().at(0)->getAbility1());
		bool bossIsAttacking = false;
		if (abilityPtr)
		{
			bossIsAttacking = abilityPtr->isAttacking();
		}

		float dist = (playerPos.x - bossPos.x) * (playerPos.x - bossPos.x) + (playerPos.y - bossPos.y) * (playerPos.y - bossPos.y);
		dist /= 10;

		if (dist <= m_attackRange)
		{
			if ((bossPos.x > playerPos.x && ani->getAnimationType() == RightAttack) || (bossPos.x < playerPos.x && ani->getAnimationType() == LeftAttack))
			{
				auto bossStats = daBoss->getComponentsOfType<StatsCP>().at(0);

				bossStats->subtracktHealth(stats->getDamage());
				if (bossStats->getHealth() <= 0)
				{
					daBoss->getComponentsOfType<AnimatedGraphicsCP<Boss_Animationtype>>().at(0)->setDying();
				}
				else if (!bossIsAttacking)
				{
					daBoss->getComponentsOfType<AnimatedGraphicsCP<Boss_Animationtype>>().at(0)->setHit();
				}
			}
		}
	}

	for (auto it = m_enemies.begin(); it != m_enemies.end();)
	{
		if (!it->expired())
		{
			newEnemies.push_back(*it);

			auto enemy = it->lock();
			auto enemyStats = enemy->getComponentsOfType<StatsCP>().at(0);
			enemyPos = enemy->getComponentsOfType<TransformationCP>().at(0)->getPosition();

			distance = (playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + (playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y);
			distance /= 10;

			if (distance <= m_attackRange)
			{
				if ((enemyPos.x > playerPos.x && ani->getAnimationType() == RightAttack) || (enemyPos.x < playerPos.x && ani->getAnimationType() == LeftAttack))
				{
					enemyStats->subtracktHealth(stats->getDamage());

					if (enemyStats->getHealth() <= 0)
					{
						enemy->getComponentsOfType<AnimatedGraphicsCP<Enemy_Animationtype>>().at(0)->setDying();
						AssetManager::getInstance().Music["Bones"]->play();
					}
					else
					{
						enemy->getComponentsOfType<AnimatedGraphicsCP<Enemy_Animationtype>>().at(0)->setHit();
					}
				}
			}
		}
		++it;
	}
	m_enemies = newEnemies;
}

template<typename T>
void PlayerAttackCP<T>::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		auto transf = gameObject.lock()->getComponentsOfType<TransformationCP>().at(0);
		auto ani = gameObject.lock()->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0);
		auto stats = gameObject.lock()->getComponentsOfType<StatsCP>().at(0);
		auto input = gameObject.lock()->getComponentsOfType<InputCP>().at(0);

		if (!m_attackLock && !m_hasAttacked)
		{
			if (!input->getInputLockState())
			{
				if (std::is_same_v<T, sf::Keyboard::Key>)
				{
					if (InputManager::getInstance().getKeyDown(static_cast<sf::Keyboard::Key>(m_attackKey)))
					{
						auto dodge = gameObject.lock()->getComponentsOfType<DashCP<sf::Keyboard::Key>>().at(0);
						dodge->setDodgeLock(true);
						doAttack(transf, ani, stats, input);
					}
				}
				else if (std::is_same_v<T, GamepadButton>)
				{
					auto movementCP = gameObject.lock()->getComponentsOfType<MovementInputGamepadCP>().at(0);

					if (movementCP->isGamepadConnected())
					{
						if (sf::Joystick::isButtonPressed(movementCP->getControllerNr(), static_cast<GamepadButton>(m_attackKey)))
						{
							auto dodge = gameObject.lock()->getComponentsOfType<DashCP<GamepadButton>>().at(0);
							dodge->setDodgeLock(true);
							doAttack(transf, ani, stats, input);
							m_useController = true;
						}
					}
				}
			}
		}
		
		if (m_attackTimer > 0.2f && m_hasAttacked)
		{
			if (m_animationLocked)
			{
				ani->setAnimationSpeed(m_originalAnimationSpeed);
				ani->toggleAnimationLock();
				m_animationLocked = false;
				if (m_useController)
					gameObject.lock()->getComponentsOfType<DashCP<GamepadButton>>().at(0)->setDodgeLock(false);
				else
					gameObject.lock()->getComponentsOfType < DashCP < sf::Keyboard::Key >> ().at(0)->setDodgeLock(false);
				/*if (lastAnimation == LeftAttack || lastAnimation == LeftDodge)
				{
					ani->setAnimationType(lastAnimation);
				}
				else if (lastAnimation == RightAttack || lastAnimation == RightDodge)
				{
					ani->setAnimationType(lastAnimation);
				}*/

				//ani->setAnimationType(lastAnimation);
			}
			if (m_inputLocked)
			{
				input->toggleInputLock();
				m_inputLocked = false;
			}

			if (m_attackCooldown > 0.4f && m_hasAttacked)
			{
				m_hasAttacked = false;
			}
		}
		else if (m_hasAttacked)
		{
			transf->setVelocity(0);
		}

		m_attackTimer += deltaTime;
		m_attackCooldown += deltaTime;
	}
}
