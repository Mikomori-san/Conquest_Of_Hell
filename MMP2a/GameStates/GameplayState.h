#pragma once
#include "GameState.h"
#include <unordered_map>

class GameObject;

using GameObjectPtr = std::shared_ptr<GameObject>;
using TexturePtr = std::shared_ptr<sf::Texture>;
using SpritePtr = std::shared_ptr<sf::Sprite>;

class GameplayState : public GameState
{
public:
	void init(sf::RenderWindow& rWindow) override;
	void exit() override;
	void update(float deltaTime) override;
	void render() override;

	bool hasWon = false;
	bool hasLost = false;
private:
	std::shared_ptr<sf::RenderWindow> window;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::map<std::string, std::vector<int>> spriteSheetCounts;

	bool slainBoss;
	bool slainPlayer;
	std::shared_ptr<sf::Music> cot;
	void loadMap(std::string name, const sf::Vector2f& offset);
	void createPlayers(tson::Object& object, tson::Layer group);
	GameObjectPtr createEnemies(tson::Object& object, tson::Layer group, std::string id);
	void createBoundary(tson::Object& object, tson::Layer group);
	void createSpawner(tson::Object& object, tson::Layer group, sf::Vector2i& aStarGridSize, std::vector<sf::Vector2i>& unMovablePositions, int mapTileSize);
	void createBoss(tson::Object& object, tson::Layer group);

	//template<typename T>
	//void doLeftoverComponents(T playerAttackCP, sf::Vector2i aStarGridSize, std::vector<sf::Vector2i> unMovablePositions, int mapTileSize);

	std::unordered_map<std::string, TexturePtr> m_tileSetTexture;
	const std::filesystem::path m_resourcePath{ "Assets" };

	std::unordered_map<std::string, GameObjectPtr> m_objects;
	int mapTileSize;
	sf::View view;
	sf::View originalView;
};