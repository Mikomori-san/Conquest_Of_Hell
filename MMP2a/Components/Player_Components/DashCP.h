//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Input_Components/InputCP.h"
#include "../Input_Components/MovementInputGamepadCP.h"
#include "DashCP.h"
#include "../StatsCP.h"
#include "../Transformation_Components/TransformationCP.h"

template <typename T>
class DashCP : public Component
{
public:
	DashCP(std::weak_ptr<GameObject> gameObject, std::string id, T incDashKey, int animSpeed) : Component(gameObject, id), dashKey(incDashKey), originalAnimationSpeed(animSpeed) {};
	
	void init() override;
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->m_componentId; };
	void setComponentId(std::string id) override { this->m_componentId = id; };
	void setDodgeLock(bool dl) { dodgeLock = dl; }
	bool getHasIFrames() { return hasCurrentlyIFrames; };
private:
	T dashKey;

	float dashTimer;
	float dashCooldown;
	float iFramesTimer;
	bool hasDashed;
	bool hasCurrentlyIFrames;
	bool inputLocked;
	bool animationLocked;
	float originalMovementSpeed;
	float originalAnimationSpeed;
	Player_Animationtype lastAnimation;
	void doDash(std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani, std::shared_ptr<StatsCP> stats, std::shared_ptr<TransformationCP> transf, std::shared_ptr<InputCP> input);
	bool dodgeLock = false;
	bool animationSpeedReset = false;
};

template <typename T>
void DashCP<T>::doDash(std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani, std::shared_ptr<StatsCP> stats, std::shared_ptr<TransformationCP> transf, std::shared_ptr<InputCP> input)
{
	if (!hasDashed)
	{
		hasDashed = true;
		dashTimer = 0;
		iFramesTimer = 0;
		dashCooldown = 0;
		hasCurrentlyIFrames = true;
		inputLocked = true;
		animationSpeedReset = false;
		//set player animation to dodging, toggle IFrames, set speed to enhanced, lock movement
		lastAnimation = ani->getAnimationType();
		if (!ani->isAnimationLock())
		{
			if (lastAnimation == Player_Animationtype::Left || lastAnimation == Player_Animationtype::LeftIdle || lastAnimation == Player_Animationtype::LeftAttack || lastAnimation == Player_Animationtype::LeftDodge)
				ani->setAnimationType(Player_Animationtype::LeftDodge);
			else
			{
				ani->setAnimationType(Player_Animationtype::RightDodge);
			}
		}
		
		animationLocked = true;
		ani->resetAnimationTimeIndex();
  		ani->setAnimationSpeed(ani->getAnimationSpeed() * 3.5f);

		ani->toggleAnimationLock(); //toggled to true
		input->toggleInputLock(); //toggled to true
		stats->toggleIFrames(); //toggled to true;
		originalMovementSpeed = transf->getOriginalVelocity();
		transf->setOriginalVelocity(originalMovementSpeed * 3);
	}
}

template <typename T>
void DashCP<T>::init()
{
	dashCooldown = 0;
	dashTimer = 0;
	iFramesTimer = 0;
	hasDashed = false;
	hasCurrentlyIFrames = false;
	inputLocked = false;
	animationLocked = false;
}

template <typename T>
void DashCP<T>::update(float deltaTime)
{
	if(!dodgeLock && !m_gameObject.expired())
	{
		std::shared_ptr<GameObject> go = m_gameObject.lock();
		std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani = go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0);
		std::shared_ptr<StatsCP> stats = go->getComponentsOfType<StatsCP>().at(0);
		std::shared_ptr<TransformationCP> transf = go->getComponentsOfType<TransformationCP>().at(0);
		std::shared_ptr<InputCP> input = go->getComponentsOfType<InputCP>().at(0);

		if (!dodgeLock)
		{
			if (std::is_same_v<T, sf::Keyboard::Key>)
			{
				if (InputManager::getInstance().getKeyDown(static_cast<sf::Keyboard::Key>(dashKey)))
					doDash(ani, stats, transf, input);
			}
			else if (std::is_same_v<T, GamepadButton>)
			{
				if (go->getComponentsOfType<MovementInputGamepadCP>().at(0)->isGamepadConnected())
				{
					if (sf::Joystick::isButtonPressed(go->getComponentsOfType<MovementInputGamepadCP>().at(0)->getControllerNr(), static_cast<GamepadButton>(dashKey)))
						doDash(ani, stats, transf, input);
				}
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

			if (!animationSpeedReset)
			{
				ani->setAnimationSpeed(originalAnimationSpeed);
				animationSpeedReset = true;
			}
					

			if (dashCooldown > 0.5f && hasDashed)
			{
				hasDashed = false;
			}
		}

		if (iFramesTimer > 0.25f && hasCurrentlyIFrames)
		{
			hasCurrentlyIFrames = false;
			stats->toggleIFrames();
		}
	}
	dashTimer += deltaTime;
	iFramesTimer += deltaTime;
	dashCooldown += deltaTime;
}