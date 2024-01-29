#include "stdafx.h"
#include "../StatsCP.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "DashCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "../Input_Components/InputCP.h"

void DashCP::init()
{
	dashCooldown = 0;
	dashTimer = 0;
	iFramesTimer = 0;
	hasDashed = false;
	hasCurrentlyIFrames = false;
	inputLocked = false;
	animationLocked = false;
}

void DashCP::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		std::shared_ptr<AnimatedGraphicsCP> ani = go->getComponentsOfType<AnimatedGraphicsCP>().at(0);
		std::shared_ptr<StatsCP> stats = go->getComponentsOfType<StatsCP>().at(0);
		std::shared_ptr<TransformationCP> transf = go->getComponentsOfType<TransformationCP>().at(0);
		std::shared_ptr<InputCP> input = go->getComponentsOfType<InputCP>().at(0);

		if (InputManager::getInstance().getKeyDown(dashKey))
		{
			if (!hasDashed)
			{
				hasDashed = true;
				dashTimer = 0;
				iFramesTimer = 0;
				dashCooldown = 0;
				hasCurrentlyIFrames = true;
				inputLocked = true;
				//set player animation to dodging, toggle IFrames, set speed to enhanced, lock movement
				lastAnimation = ani->getAnimationType();
				if (lastAnimation == Player_Animationtype::Left || lastAnimation == Player_Animationtype::LeftIdle || lastAnimation == Player_Animationtype::LeftAttack || lastAnimation == Player_Animationtype::LeftDodge)
					ani->setAnimationType(Player_Animationtype::LeftDodge);
				else
				{
					ani->setAnimationType(Player_Animationtype::RightDodge);
				}
				animationLocked = true;
				ani->resetAnimationTimeIndex();
				originalAnimationSpeed = ani->getAnimationSpeed();
				ani->setAnimationSpeed(ani->getAnimationSpeed() * 3.5f);

				ani->toggleAnimationLock(); //toggled to true
				input->toggleInputLock(); //toggled to true
				stats->toggleIFrames(); //toggled to true;
				originalMovementSpeed = transf->getOriginalVelocity();
				transf->setOriginalVelocity(originalMovementSpeed * 3);
			}
		}

		if (dashTimer > 0.3f && hasDashed)
		{
			//reset player animation & speed to normal & stop locking, toggleIFrames

			if (animationLocked)
			{
				ani->toggleAnimationLock();
				animationLocked = false;
				ani->setAnimationType(lastAnimation);
			}

			if (inputLocked)
			{
				input->toggleInputLock();
				inputLocked = false;
			}

			transf->setOriginalVelocity(originalMovementSpeed);
			ani->setAnimationSpeed(originalAnimationSpeed);

			if (dashCooldown > 0.5f && hasDashed)
			{
				hasDashed = false;
			}
		}

		if (iFramesTimer > 0.15f && hasCurrentlyIFrames)
		{
			hasCurrentlyIFrames = false;
			stats->toggleIFrames();
		}
	}

	dashTimer += deltaTime;
	iFramesTimer += deltaTime;
	dashCooldown += deltaTime;
}