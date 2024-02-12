//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "DebugDraw.h"
#include "FPS.h"
#include "GameObject.h"
#include "GameStates/GameplayState.h"
#include "GameStates/LooseState.h"
#include "GameStates/MenuState.h"
#include "GameStates/WinState.h"
#include "Manager/AssetManager.h"
#include "Manager/GameStateManager.h"
#include "Manager/InputManager.h"

class Game
{
public:
	void run();
	Game();
private:
	const int WIDTH = 1792;
	const int HEIGHT = 1024;
	const char* TITLE = "SLLOTH-SOFT";
	const sf::Color BG_COLOR = sf::Color(50, 50, 50);
	sf::Clock m_clock;
	sf::VideoMode m_mode;
	Fps m_fps;
	sf::RenderWindow m_window;

	void closeGame(const sf::Event& event);
	void initialize();
	void handleEvents();

	void draw();
	void update(float deltaTime);
	int m_state = 0;
};