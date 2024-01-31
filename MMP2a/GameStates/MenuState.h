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

	std::shared_ptr<sf::RenderWindow> window;
	sf::Sprite backgroundSprite;
	sf::Sprite titleSprite;
	sf::Texture bgTexture;
	sf::Texture titleTexture;
};