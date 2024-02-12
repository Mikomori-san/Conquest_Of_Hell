//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../GameObject.h"
#include "../Component.h"

class SpawnerCP : public Component
{
public:
	SpawnerCP(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<GameObject> enemy, std::shared_ptr<GameObject> enemy2, std::weak_ptr<GameObject> go, std::string id, std::string enemyName, int maxEnemies, float spawnTime,
		sf::Vector2i aStarGridSize, std::vector<sf::Vector2i>& unMovablePositions, int mapTileSize)
		: Component(go, id), availableEnemies(std::vector<std::shared_ptr<GameObject>>{enemy, enemy2}), m_gameObjects(gameObjects), m_enemyName(enemyName), m_spawnTime(spawnTime), m_maxEnemies(maxEnemies), aStarGridSize(aStarGridSize),
		unMovablePositions(unMovablePositions), mapTileSize(mapTileSize)
	{};


	virtual ~SpawnerCP() = default;

	virtual std::string getComponentId() override { return componentId; }
	virtual void setComponentId(std::string id) override { this->componentId = id; }

	virtual void update(float deltaTime) override;
	virtual void init() override;

	void spawnEnemy();

private:
	std::vector<std::shared_ptr<GameObject>>& m_gameObjects;
	int m_maxEnemies = 2;
	float m_spawnTime = 10.f;
	std::string m_enemyName = "";
	float spawnTimer = 0.f;
	std::vector<std::shared_ptr<GameObject>> availableEnemies;
	sf::Vector2i aStarGridSize; 
	std::vector<sf::Vector2i> unMovablePositions;
	int mapTileSize;

	void late_init_if_enemy_spawned(std::shared_ptr<GameObject> player);
	bool enemy1Spawned = false;
	bool enemy2Spawned = false;
};

