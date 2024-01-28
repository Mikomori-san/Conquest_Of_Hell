#pragma once
#include "../Component.h"
class SteeringCP : public Component
{
public:
	SteeringCP(std::weak_ptr<GameObject> gameObject, std::string id) : Component(gameObject, id) {};
	void update(float deltaTime) override;
	std::string getComponentId() override;
	void setComponentId(std::string id) override;
	void init() override;

	void setDestination(sf::Vector2f incDest) { destination = incDest; }

private:
	sf::Vector2f destination;
};