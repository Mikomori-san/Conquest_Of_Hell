#include "stdafx.h"
#include "Game.h"

Game::Game() : window(sf::VideoMode(WIDTH, HEIGHT), TITLE)
{
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
}

void Game::closeGame(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		window.close();
}

void Game::initialize()
{
	InputManager::getInstance().init(window);

	GameStateManager::getInstance().reg("Menu", std::make_shared<MenuState>());
	GameStateManager::getInstance().reg("Gameplay", std::make_shared<GameplayState>());
	GameStateManager::getInstance().reg("Exit", std::make_shared<ExitState>());

	GameStateManager::getInstance().setState("Gameplay", window);
}

void Game::run()
{
	initialize();

	while (window.isOpen())
	{
		auto deltaTime = clock.restart().asSeconds();

		handleEvents();
		update(deltaTime);
		draw();
	}
}

void Game::handleEvents()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
			window.close();
		
		InputManager::getInstance().handleEvents(event);
	}
}

void Game::draw()
{
	window.clear(BG_COLOR);

	GameStateManager::getInstance().render();
	
	window.display();
}

void Game::update(float deltaTime)
{
	if (InputManager::getInstance().getKeyDown(sf::Keyboard::Num1))
		GameStateManager::getInstance().setState("Menu", window);
	else if (InputManager::getInstance().getKeyDown(sf::Keyboard::Num2))
		GameStateManager::getInstance().setState("Gameplay", window);

	GameStateManager::getInstance().update(deltaTime);

	InputManager::getInstance().update();

	std::ostringstream ss;
	m_fps.update();
	ss << "SLLOTH-SOFT | FPS: " << m_fps.getFps();

	window.setTitle(ss.str());
}