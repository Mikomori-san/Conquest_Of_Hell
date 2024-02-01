#include "stdafx.h"
#include "MeleeBA.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Player_Components/DashCP.h"
#include "../StatsCP.h"
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
			std::dynamic_pointer_cast<AnimatedGraphicsCP<Boss_Animationtype>>(boss->getComponentsOfType<AnimatedGraphicsCP<Boss_Animationtype>>().at(0));
		if (animation)
		{
			int currFrame = animation->getAnimationFrame();
			//if (currFrame == 8)
			//{
			//	attackPlayer();
			//}
			//resets animation to idle when attack animation is over
			if (currFrame < lastAnimationFrame)
			{
				animation->setAnimationType(Boss_Animationtype::Idle);
				animation->setAnimationSpeed(defaultAnimationSpeed);
				lastAnimationFrame = 0;
				attackPlayer();
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
		std::shared_ptr<DashCP<sf::Keyboard::Key>> dash = std::dynamic_pointer_cast<DashCP<sf::Keyboard::Key>>(go->getComponentsOfType<DashCP<sf::Keyboard::Key>>().at(0));
		std::shared_ptr<StatsCP> stats = std::dynamic_pointer_cast<StatsCP>(go->getComponentsOfType<StatsCP>().at(0));
		if (dash && stats)
		{
			if (!dash->getHasIFrames())
			{
				std::cout << "Attack" << std::endl;
				stats->subtracktHealth(damage);
			}
		}
	}
}
