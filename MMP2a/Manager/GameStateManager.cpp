//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "GameStateManager.h"

void GameStateManager::reg(std::string name, std::shared_ptr<GameState> state)
{
	m_states[name] = state;
}

void GameStateManager::setState(std::string name, sf::RenderWindow& window)
{
	std::shared_ptr<GameState> state = m_states[name];

	if (state != m_currentState)
	{
		if (m_currentState)
		{
			m_currentState->exit();
		}
		m_currentState = state;

		m_currentState->init(window);
	}
}

void GameStateManager::update(float deltaTime)
{
	m_currentState->update(deltaTime);
}

void GameStateManager::render()
{
	m_currentState->render();
}
