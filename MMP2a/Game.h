#include "GameObject.h"
#include "Manager/AssetManager.h"
#include "Manager/InputManager.h"
#include "DebugDraw.h"
#include "GameObject.h"
#include "Manager/GameStateManager.h"
#include "GameStates/GameplayState.h"
#include "GameStates/MenuState.h"
#include "GameStates/ExitState.h"
#include "FPS.h"
#pragma once

class Game
{
public:
	void run();
	Game();
private:
	const int WIDTH = 1792;
	const int HEIGHT = 1024;
	const char* TITLE = "SLLOTH-SOFT";
	sf::Color BG_COLOR = sf::Color(50, 50, 50);
	sf::Clock clock;
	sf::VideoMode mode;
	sf::RenderWindow window;
	Fps m_fps;

	void closeGame(const sf::Event& event);
	void initialize();
	void handleEvents();

	void draw();
	void update(float deltaTime);
};