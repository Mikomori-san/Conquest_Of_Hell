#include "stdafx.h"
#include "MeleeBA.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Player_Components/DashCP.h"
#include "../StatsCP.h"
#include "../../Enums/MeeleIndicator_Animationtype.h"
#include "BossAttackCP.h"
void MeleeBA::init()
{
	abilityType = Melee;
}

void MeleeBA::update(float deltaTime)
{
	if (attacking && !bossPtr.expired())
	{
		std::shared_ptr<GameObject> boss = bossPtr.lock();
		std::shared_ptr<AnimatedGraphicsCP<Boss_Animationtype>> animation =
			std::dynamic_pointer_cast<AnimatedGraphicsCP<Boss_Animationtype>>
			(boss->getComponentsOfType<AnimatedGraphicsCP<Boss_Animationtype>>().at(0));

		if (animation)
		{
			int currFrame = animation->getAnimationFrame();
			boss->getComponentsOfType<BossAttackCP>().at(0)->getMeeleeInd()->setAnimationFrame(currFrame);
			//resets animation to idle when attack animation is over
			if (currFrame < lastAnimationFrame)
			{
				animation->setAnimationType(Boss_Animationtype::Idle);
				animation->setAnimationSpeed(defaultAnimationSpeed);
				attackPlayer();
				lastAnimationFrame = 0;
			}
			else
			{
				lastAnimationFrame = currFrame;
			}
		}
	}
}

void MeleeBA::execute()
{
	attacking = true;
	if (!bossPtr.expired())
	{
		std::shared_ptr<GameObject> boss = bossPtr.lock();
		std::shared_ptr<AnimatedGraphicsCP<Boss_Animationtype>> animation = 
			std::dynamic_pointer_cast<AnimatedGraphicsCP<Boss_Animationtype>>(boss->getComponentsOfType<AnimatedGraphicsCP<Boss_Animationtype>>().at(0));

		if (animation)
		{
			animation->setAnimationType(Boss_Animationtype::Attack);
			animation->resetAnimationTimeIndex();
			animation->resetAnimationFrame();
			defaultAnimationSpeed = animation->getAnimationSpeed();
			animation->setAnimationSpeed(8);
		}
	}
}

void MeleeBA::attackPlayer()
{
	attacking = false;
	if (!playerPtr.expired())
	{
		std::shared_ptr<GameObject> go = playerPtr.lock();
		if (go->getComponentsOfType<MovementInputGamepadCP>().size() > 0)
		{
			std::shared_ptr<DashCP<GamepadButton>> dash = go->getComponentsOfType<DashCP<GamepadButton>>().at(0);
			std::shared_ptr<StatsCP> stats = go->getComponentsOfType<StatsCP>().at(0);
			if (dash && stats)
			{
				if (!dash->getHasIFrames())
				{
					std::cout << "Attack" << std::endl;
					stats->subtracktHealth(damage);

					if (stats->getHealth() <= 0)
					{
						go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0)->setDying();
					}
					else
					{
						go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0)->setColor(sf::Color::Red);
					}
				}
			}
		}
		else
		{
			std::shared_ptr<DashCP<sf::Keyboard::Key>> dash = go->getComponentsOfType<DashCP<sf::Keyboard::Key>>().at(0);
			std::shared_ptr<StatsCP> stats = go->getComponentsOfType<StatsCP>().at(0);
			if (dash && stats)
			{
				if (!dash->getHasIFrames())
				{
					std::cout << "Attack" << std::endl;
					stats->subtracktHealth(damage);

					if (stats->getHealth() <= 0)
					{
						go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0)->setDying();
					}
					else
					{
						go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0)->setColor(sf::Color::Red);
					}
				}
			}
		}
		
	}
}
