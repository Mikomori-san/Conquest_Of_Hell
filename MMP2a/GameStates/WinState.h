//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../DebugDraw.h"
#include "../GameObject.h"
#include "../Manager/AssetManager.h"
#include "GameState.h"


class WinState : public GameState
{
public:
	void init(sf::RenderWindow& rWindow) override;
	void exit() override;
	void update(float deltaTime) override;
	void render() override;

private:
	sf::RectangleShape createButton(std::string name, sf::Vector2f size, sf::Vector2f pos, sf::Color fillColor);

	std::vector<std::shared_ptr<GameObject>> m_gameObjects;

	std::shared_ptr<sf::RenderWindow> m_window;
	sf::Sprite m_backgroundSprite;

	std::shared_ptr<sf::Music> m_cw;

	sf::RectangleShape m_startButton;
	sf::RectangleShape m_exitButton;

	sf::Sprite m_restartSprite;
	sf::Sprite m_exitSprite;

	std::vector<std::pair<sf::Text, sf::RectangleShape>> m_buttonList;
	std::map<std::string, sf::RectangleShape> m_buttonMap;

	bool m_isControllerConnected = false;
	bool m_isPlaySelected = true;
	bool m_isExitSelected = false;
};