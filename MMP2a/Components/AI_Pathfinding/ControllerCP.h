//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"
#include "States/AIState.h"

typedef GameObject;

class ControllerCP : public Component
{
public:
	ControllerCP(std::weak_ptr<GameObject> gameObject, std::string id, std::vector<std::weak_ptr<GameObject>> incPlayers) : Component(gameObject, id), players(incPlayers) { health = 0; maxHealth = 0; };
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->componentId; };
	void setComponentId(std::string id) override { this->componentId = id; };
	void init() override;
	void setPlayers(std::vector<std::weak_ptr<GameObject>> incPlayers) { players = incPlayers; }
private:
	std::vector<std::weak_ptr<GameObject>> players;
	int health;
	int maxHealth;
	std::shared_ptr<AIState> currentState;
};