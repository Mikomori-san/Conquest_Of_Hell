#pragma once
#include "Component.h"
class StatsCP : public Component
{
public:

	StatsCP(std::weak_ptr<GameObject> gameObject, std::string id, int hp, std::string incObjType) : Component(gameObject, id), health(hp), gameObjectType(incObjType) {};

	void update(float deltaTime) override;

	std::string getComponentId() override
	{
		return this->componentId;
	}

	void setComponentId(std::string id) override
	{
		this->componentId = id;
	}

	void init() override;

	void subtracktHealth(int dmg) { health -= dmg; }
	void addHealth(int healing) { health += healing; }
	int getHealth() { return health; }
	void setHealth(int newHealth) { health = newHealth; }

	void ifEnemyAddPatrolPoints(std::string i) { patrolPoints = i; }
	std::string ifEnemyGetPatrolPoints() { return this->patrolPoints; }

	std::string getObjectType() { return this->gameObjectType; }
private:
	int health = 100;
	std::string patrolPoints;				// IS ONLY SET _AND_ USED WHEN ENEMY
	std::string gameObjectType;
};