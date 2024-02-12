//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../Enums/GamepadButton.h"
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
    PlayerAttackCP(std::weak_ptr<GameObject> gameObject, std::string id, int incAR, std::vector<std::weak_ptr<GameObject>> incEnemies, T incKey) : Component(gameObject, id), enemies(incEnemies), attackKey(incKey), attackRange(incAR) {}

    void update(float deltaTime) override;
    std::string getComponentId() override { return this->m_componentId; };
    void setComponentId(std::string id) override { this->m_componentId = id; }
    void init() override;
    void addEnemy(std::weak_ptr<GameObject> enemy) { enemies.push_back(enemy); }
	void setAttackLock(bool al) { attackLock = al; }
	void setBoss(std::weak_ptr<GameObject> incBoss) { boss = incBoss; }
private:
    std::vector<std::weak_ptr<GameObject>> enemies;
    T attackKey;
    int attackRange;

	std::weak_ptr<GameObject> boss;

    bool hasAttacked;
    bool inputLocked;
    bool animationLocked;

    float attackTimer;
    float attackCooldown;

    int originalAnimationSpeed;
    Player_Animationtype lastAnimation;

    sf::Color lastColor;

    void doAttack(std::shared_ptr<TransformationCP> trans, std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani,
        std::shared_ptr<StatsCP> stats, std::shared_ptr<InputCP> input);

	bool attackLock = false;
	bool useController = false;
};


template<typename T>
void PlayerAttackCP<T>::init()
{
	hasAttacked = false;
}

template<typename T>
void PlayerAttackCP<T>::doAttack(std::shared_ptr<TransformationCP> transf, std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani,
	std::shared_ptr<StatsCP> stats, std::shared_ptr<InputCP> input)
{
	float distance;
	sf::Vector2f playerPos;
	sf::Vector2f enemyPos;

	hasAttacked = true;
	attackTimer = 0;
	attackCooldown = 0;
	inputLocked = true;
	lastAnimation = ani->getAnimationType();
	animationLocked = true;
		
	if (!ani->isAnimationLock())
	{
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
	}
	input->toggleInputLock();

	transf->setVelocity(0);

	playerPos = transf->getPosition();
	std::vector<std::weak_ptr<GameObject>> newEnemies;

	if (!boss.expired())
	{
		auto daBoss = boss.lock();
		auto bossPos = daBoss->getComponentsOfType<TransformationCP>().at(0)->getPosition();
		std::shared_ptr<MeleeBA> abilityPtr = std::dynamic_pointer_cast<MeleeBA>(daBoss->getComponentsOfType<BossAttackCP>().at(0)->getAbility1());
		bool bossIsAttacking = false;
		if (abilityPtr)
		{
			bossIsAttacking = abilityPtr->isAttacking();
		}

		float dist = (playerPos.x - bossPos.x) * (playerPos.x - bossPos.x) + (playerPos.y - bossPos.y) * (playerPos.y - bossPos.y);
		dist /= 10;

		if (dist <= attackRange)
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

	for (auto it = enemies.begin(); it != enemies.end();)
	{
		if (!it->expired())
		{
			newEnemies.push_back(*it);

			auto enemy = it->lock();
			auto enemyStats = enemy->getComponentsOfType<StatsCP>().at(0);
			enemyPos = enemy->getComponentsOfType<TransformationCP>().at(0)->getPosition();

			distance = (playerPos.x - enemyPos.x) * (playerPos.x - enemyPos.x) + (playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y);
			distance /= 10;

			if (distance <= attackRange)
			{
				if ((enemyPos.x > playerPos.x && ani->getAnimationType() == RightAttack) || (enemyPos.x < playerPos.x && ani->getAnimationType() == LeftAttack))
				{
					enemyStats->subtracktHealth(stats->getDamage());

					if (enemyStats->getHealth() <= 0)
					{
						enemy->getComponentsOfType<AnimatedGraphicsCP<Enemy_Animationtype>>().at(0)->setDying();
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
	enemies = newEnemies;
}

template<typename T>
void PlayerAttackCP<T>::update(float deltaTime)
{
	if (!m_gameObject.expired())
	{
		auto transf = m_gameObject.lock()->getComponentsOfType<TransformationCP>().at(0);
		auto ani = m_gameObject.lock()->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0);
		auto stats = m_gameObject.lock()->getComponentsOfType<StatsCP>().at(0);
		auto input = m_gameObject.lock()->getComponentsOfType<InputCP>().at(0);

		if (!attackLock && !hasAttacked)
		{
			if (!input->getInputLockState())
			{
				if (std::is_same_v<T, sf::Keyboard::Key>)
				{
					if (InputManager::getInstance().getKeyDown(static_cast<sf::Keyboard::Key>(attackKey)))
					{
						auto dodge = m_gameObject.lock()->getComponentsOfType<DashCP<sf::Keyboard::Key>>().at(0);
						dodge->setDodgeLock(true);
						doAttack(transf, ani, stats, input);
					}
				}
				else if (std::is_same_v<T, GamepadButton>)
				{
					auto movementCP = m_gameObject.lock()->getComponentsOfType<MovementInputGamepadCP>().at(0);

					if (movementCP->isGamepadConnected())
					{
						if (sf::Joystick::isButtonPressed(movementCP->getControllerNr(), static_cast<GamepadButton>(attackKey)))
						{
							auto dodge = m_gameObject.lock()->getComponentsOfType<DashCP<GamepadButton>>().at(0);
							dodge->setDodgeLock(true);
							doAttack(transf, ani, stats, input);
							useController = true;
						}
					}
				}
			}
		}
		
		if (attackTimer > 0.2f && hasAttacked)
		{
			if (animationLocked)
			{
				ani->setAnimationSpeed(originalAnimationSpeed);
				ani->toggleAnimationLock();
				animationLocked = false;
				if (useController)
					m_gameObject.lock()->getComponentsOfType<DashCP<GamepadButton>>().at(0)->setDodgeLock(false);
				else
					m_gameObject.lock()->getComponentsOfType < DashCP < sf::Keyboard::Key >> ().at(0)->setDodgeLock(false);
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
			if (inputLocked)
			{
				input->toggleInputLock();
				inputLocked = false;
			}

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
