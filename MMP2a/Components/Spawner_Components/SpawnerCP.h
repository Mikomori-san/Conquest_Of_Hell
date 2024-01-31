#pragma once
#include "../../GameObject.h"
#include "../Component.h"

class SpawnerCP : public Component
{
public:
	SpawnerCP(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<GameObject> enemy, std::shared_ptr<GameObject> enemy2, std::weak_ptr<GameObject> go, std::string id, std::string enemyName, int maxEnemies, float spawnTime)
		: Component(go, id), m_enemy(enemy), m_enemy2(enemy2), m_gameObjects(gameObjects), m_enemyName(enemyName), m_spawnTime(spawnTime), m_maxEnemies(maxEnemies)
	{};


	virtual ~SpawnerCP() = default;

	virtual std::string getComponentId() override { return componentId; }
	virtual void setComponentId(std::string id) override { this->componentId = id; }

	virtual void update(float deltaTime) override;
	virtual void init() override;

	void spawnEnemy();

	//std::vector<std::weak_ptr<GameObject>> m_enemiesList;
	std::vector<int> m_enemyIdList = {};

private:
	std::vector<std::shared_ptr<GameObject>>& m_gameObjects;
	int m_maxEnemies = 2;
	float m_spawnTime = 10.f;
	std::string m_enemyName = "";
	float spawnTimer = 0.f;
	std::shared_ptr<GameObject> m_enemy;
	std::shared_ptr<GameObject> m_enemy2;
};

