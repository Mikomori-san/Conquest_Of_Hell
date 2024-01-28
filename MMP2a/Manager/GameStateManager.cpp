#include "stdafx.h"
#include "GameStateManager.h"

void GameStateManager::reg(std::string name, std::shared_ptr<GameState> state)
{
	states[name] = state;
}

void GameStateManager::setState(std::string name, sf::RenderWindow& window)
{
	std::shared_ptr<GameState> state = states[name];

	if (state != currentState)
	{
		if (currentState)
		{
			currentState->exit();
		}
		currentState = state;

		currentState->init(window);
	}
}

void GameStateManager::update(float deltaTime)
{
	currentState->update(deltaTime);
}

void GameStateManager::render()
{
	currentState->render();
}
