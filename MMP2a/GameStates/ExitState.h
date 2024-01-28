#pragma once
#include "GameState.h"

class ExitState : public GameState
{
public:
	void init(sf::RenderWindow& rWindow) override;
	void exit() override;
	void update(float deltaTime) override;
	void render() override;
};