#pragma once
#include "../Component.h"
#include "../../Manager/InputManager.h"
#include "../../Enums/Direction.h"

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

	virtual void toggleInputLock() { inputLock = inputLock ? false : true; }
	virtual bool getInputLockState() { return inputLock; }

protected:
	bool inputLock = false;
	Direction lastDirection;
	sf::Vector2f lastVec;
	float vel;
	bool hadInput = false;
};