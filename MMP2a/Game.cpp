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
	sf::VideoMode fullscreenMode(1920, 1080);
	window.create(fullscreenMode, "Fullscreen Window", sf::Style::Fullscreen);
	
	InputManager::getInstance().init(window);

	GameStateManager::getInstance().reg("Menu", std::make_shared<MenuState>());
	GameStateManager::getInstance().reg("Gameplay", std::make_shared<GameplayState>());
	GameStateManager::getInstance().reg("Exit", std::make_shared<ExitState>());

	GameStateManager::getInstance().setState("Menu", window);

	InputManager::getInstance().setRenderWindow(window);
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
	if (GameStateManager::getInstance().getState()->hasClosed())
		state++;

	switch (state)
	{
	case 1:
		if(!std::dynamic_pointer_cast<GameplayState>(GameStateManager::getInstance().getState()))
			GameStateManager::getInstance().setState("Gameplay", window);
		break;
	case 2:
		if (!std::dynamic_pointer_cast<ExitState>(GameStateManager::getInstance().getState()))
			GameStateManager::getInstance().setState("Exit", window);
		break;
	default:
		break;
	}

	GameStateManager::getInstance().update(deltaTime);

	InputManager::getInstance().update();

	std::ostringstream ss;
	m_fps.update();
	ss << "SLLOTH-SOFT | FPS: " << m_fps.getFps();

	window.setTitle(ss.str());
}