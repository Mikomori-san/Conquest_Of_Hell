#pragma once
#include "../Component.h"

class DashCP : public Component
{
public:
	DashCP(std::weak_ptr<GameObject> gameObject, std::string id, sf::Keyboard::Key incDashKey) : Component(gameObject, id), dashKey(incDashKey) {};
	
	void init() override;
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->componentId; };
	void setComponentId(std::string id) override { this->componentId = id; };
	
private:
	sf::Keyboard::Key dashKey;

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
};