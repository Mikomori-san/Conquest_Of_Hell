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
	ControllerCP(std::weak_ptr<GameObject> gameObject, std::string id, std::vector<std::weak_ptr<GameObject>> incPlayers) : Component(gameObject, id), m_players(incPlayers) { m_health = 0; m_maxHealth = 0; };
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->m_componentId; };
	void setComponentId(std::string id) override { this->m_componentId = id; };
	void init() override;
	void setPlayers(std::vector<std::weak_ptr<GameObject>> incPlayers) { m_players = incPlayers; }
private:
	std::vector<std::weak_ptr<GameObject>> m_players;
	int m_health;
	int m_maxHealth;
	std::shared_ptr<AIState> m_currentState;
};