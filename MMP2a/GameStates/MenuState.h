#pragma once
#include "GameState.h"
#include "../GameObject.h"
#include "../Manager/AssetManager.h"
#include "../DebugDraw.h"

class MenuState : public GameState
{
public:
	void init(sf::RenderWindow& rWindow) override;
	void exit() override;
	void update(float deltaTime) override;
	void render() override;

private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::shared_ptr<GameObject> background1;
	std::shared_ptr<GameObject> background2;
	std::shared_ptr<GameObject> player;
	std::shared_ptr<GameObject> player2;
	std::shared_ptr<sf::RenderWindow> window;
	std::shared_ptr<GameObject> camera;
	const int TILE_SIZE = 64;

	void checkAreaBorders();
	void drawFloor(sf::Vector2f position, sf::Vector2i tiles, sf::Vector2i tileSize);
	void respawnPlayer();
	void addPlayerComponents(std::shared_ptr<GameObject> player, bool useArrowKeys);
	void addBackgroundComponents(std::shared_ptr<GameObject> player);
	void checkBackgroundPos();
};