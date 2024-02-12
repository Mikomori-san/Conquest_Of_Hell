//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "Game.h"

Game::Game() : m_window(sf::VideoMode(WIDTH, HEIGHT), TITLE)
{
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);

}

void Game::closeGame(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		m_window.close();
}

void Game::initialize()
{
	sf::VideoMode fullscreenMode(1920, 1080);
	m_window.create(fullscreenMode, "Fullscreen Window", sf::Style::Fullscreen);
	
	InputManager::getInstance().init(m_window);

	GameStateManager::getInstance().reg("Menu", std::make_shared<MenuState>());
	GameStateManager::getInstance().reg("Gameplay", std::make_shared<GameplayState>());
	GameStateManager::getInstance().reg("Win", std::make_shared<WinState>());
	GameStateManager::getInstance().reg("Loose", std::make_shared<LooseState>());

	GameStateManager::getInstance().setState("Menu", m_window);

	InputManager::getInstance().setRenderWindow(m_window);
}

void Game::run()
{
	initialize();

	while (m_window.isOpen())
	{
		auto deltaTime = m_clock.restart().asSeconds();

		handleEvents();
		update(deltaTime);
		draw();
	}
}

void Game::handleEvents()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
			m_window.close();
		
		InputManager::getInstance().handleEvents(event);
	}
}

void Game::draw()
{
	m_window.clear(BG_COLOR);

	GameStateManager::getInstance().render();
	
	m_window.display();
}

void Game::update(float deltaTime)
{
	if (GameStateManager::getInstance().getState()->hasClosed())
		m_state++;

	switch (m_state)
	{
	case 1:
		if(!std::dynamic_pointer_cast<GameplayState>(GameStateManager::getInstance().getState()))
			GameStateManager::getInstance().setState("Gameplay", m_window);
		break;
	case 2:
		if (!std::dynamic_pointer_cast<WinState>(GameStateManager::getInstance().getState()))
			GameStateManager::getInstance().setState("Win", m_window);
		break;
	case 3:
		if (!std::dynamic_pointer_cast<LooseState>(GameStateManager::getInstance().getState()))
			GameStateManager::getInstance().setState("Loose", m_window);
		break;
	default:
		break;
	}

	GameStateManager::getInstance().update(deltaTime);

	InputManager::getInstance().update();

	std::ostringstream ss;
	m_fps.update();
	std::cout << "SLLOTH-SOFT | FPS: " << m_fps.getFps() << std::endl;

	m_window.setTitle(ss.str());
}