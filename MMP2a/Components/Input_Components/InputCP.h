//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../Enums/Direction.h"
#include "../../Manager/InputManager.h"
#include "../Component.h"

class InputCP : public Component
{
public:
	InputCP(std::weak_ptr<GameObject> gameObject, std::string id)
		: Component(gameObject, id) 
	{}

	virtual ~InputCP() = default;

	virtual std::string getComponentId() override { return this->componentId; }
	virtual void setComponentId(std::string id) override { this->componentId = id; }
	virtual void update(float deltaTime) override {};
	virtual void processInput() {};

	virtual void toggleInputLock() { m_inputLock = m_inputLock ? false : true; }
	virtual bool getInputLockState() { return m_inputLock; }

protected:
	bool m_inputLock = false;
	Direction m_lastDirection;
	sf::Vector2f m_lastVec;
	float m_vel;
	bool m_hadInput = false;
};