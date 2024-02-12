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
	DashCP(std::weak_ptr<GameObject> gameObject, std::string id, T incDashKey, int animSpeed) : Component(gameObject, id), m_dashKey(incDashKey), m_originalAnimationSpeed(animSpeed) {};
	
	void init() override;
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->componentId; };
	void setComponentId(std::string id) override { this->componentId = id; };
	void setDodgeLock(bool dl) { m_dodgeLock = dl; }
	bool getHasIFrames() { return m_hasCurrentlyIFrames; };
private:
	T m_dashKey;

	float m_dashTimer;
	float m_dashCooldown;
	float m_iFramesTimer;
	bool m_hasDashed;
	bool m_hasCurrentlyIFrames;
	bool m_inputLocked;
	bool m_animationLocked;
	float m_originalMovementSpeed;
	float m_originalAnimationSpeed;
	Player_Animationtype m_lastAnimation;
	void doDash(std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani, std::shared_ptr<StatsCP> stats, std::shared_ptr<TransformationCP> transf, std::shared_ptr<InputCP> input);
	bool m_dodgeLock = false;
	bool m_animationSpeedReset = false;
};

template <typename T>
void DashCP<T>::doDash(std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani, std::shared_ptr<StatsCP> stats, std::shared_ptr<TransformationCP> transf, std::shared_ptr<InputCP> input)
{
	if (!m_hasDashed)
	{
		m_hasDashed = true;
		m_dashTimer = 0;
		m_iFramesTimer = 0;
		m_dashCooldown = 0;
		m_hasCurrentlyIFrames = true;
		m_inputLocked = true;
		m_animationSpeedReset = false;
		//set player animation to dodging, toggle IFrames, set speed to enhanced, lock movement
		m_lastAnimation = ani->getAnimationType();
		if (!ani->isAnimationLock())
		{
			if (m_lastAnimation == Player_Animationtype::Left || m_lastAnimation == Player_Animationtype::LeftIdle || m_lastAnimation == Player_Animationtype::LeftAttack || m_lastAnimation == Player_Animationtype::LeftDodge)
				ani->setAnimationType(Player_Animationtype::LeftDodge);
			else
			{
				ani->setAnimationType(Player_Animationtype::RightDodge);
			}
		}
		
		m_animationLocked = true;
		ani->resetAnimationTimeIndex();
  		ani->setAnimationSpeed(ani->getAnimationSpeed() * 3.5f);

		ani->toggleAnimationLock(); //toggled to true
		input->toggleInputLock(); //toggled to true
		stats->toggleIFrames(); //toggled to true;
		m_originalMovementSpeed = transf->getOriginalVelocity();
		transf->setOriginalVelocity(m_originalMovementSpeed * 3);
	}
}

template <typename T>
void DashCP<T>::init()
{
	m_dashCooldown = 0;
	m_dashTimer = 0;
	m_iFramesTimer = 0;
	m_hasDashed = false;
	m_hasCurrentlyIFrames = false;
	m_inputLocked = false;
	m_animationLocked = false;
}

template <typename T>
void DashCP<T>::update(float deltaTime)
{
	if(!m_dodgeLock && !gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		std::shared_ptr<AnimatedGraphicsCP<Player_Animationtype>> ani = go->getComponentsOfType<AnimatedGraphicsCP<Player_Animationtype>>().at(0);
		std::shared_ptr<StatsCP> stats = go->getComponentsOfType<StatsCP>().at(0);
		std::shared_ptr<TransformationCP> transf = go->getComponentsOfType<TransformationCP>().at(0);
		std::shared_ptr<InputCP> input = go->getComponentsOfType<InputCP>().at(0);

		if (!m_dodgeLock)
		{
			if (std::is_same_v<T, sf::Keyboard::Key>)
			{
				if (InputManager::getInstance().getKeyDown(static_cast<sf::Keyboard::Key>(m_dashKey)))
					doDash(ani, stats, transf, input);
			}
			else if (std::is_same_v<T, GamepadButton>)
			{
				if (go->getComponentsOfType<MovementInputGamepadCP>().at(0)->isGamepadConnected())
				{
					if (sf::Joystick::isButtonPressed(go->getComponentsOfType<MovementInputGamepadCP>().at(0)->getControllerNr(), static_cast<GamepadButton>(m_dashKey)))
						doDash(ani, stats, transf, input);
				}
			}
		}
			
		if (m_dashTimer > 0.3f && m_hasDashed)
		{
			//reset player animation & speed to normal & stop locking, toggleIFrames

			if (m_animationLocked)
			{
				ani->toggleAnimationLock();
				m_animationLocked = false;
				ani->setAnimationType(m_lastAnimation);
			}

			if (m_inputLocked)
			{
				input->toggleInputLock();
				m_inputLocked = false;
			}

			transf->setOriginalVelocity(m_originalMovementSpeed);

			if (!m_animationSpeedReset)
			{
				ani->setAnimationSpeed(m_originalAnimationSpeed);
				m_animationSpeedReset = true;
			}
					

			if (m_dashCooldown > 0.5f && m_hasDashed)
			{
				m_hasDashed = false;
			}
		}

		if (m_iFramesTimer > 0.25f && m_hasCurrentlyIFrames)
		{
			m_hasCurrentlyIFrames = false;
			stats->toggleIFrames();
		}
	}
	m_dashTimer += deltaTime;
	m_iFramesTimer += deltaTime;
	m_dashCooldown += deltaTime;
}