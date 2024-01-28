#pragma once
#include "Component.h"
#include "../Enums/Direction.h"

class DecisionHandlerCP : public Component
{
public:
	DecisionHandlerCP(std::shared_ptr<GameObject> gameObject, std::string id) : Component(gameObject, id) {};
	void update(float deltaTime) override;
	std::string getComponentId() override;
	void setComponentId(std::string id) override;
	void init() override;

	void handleMovement(Direction dir, sf::Vector2f dirVec, float vel);
};