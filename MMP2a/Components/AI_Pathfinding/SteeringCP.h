//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
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

	void setDestination(sf::Vector2f incDest) { m_destination = incDest; }

private:
	sf::Vector2f m_destination;
};