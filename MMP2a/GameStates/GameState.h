#pragma once
#include "../stdafx.h"

class GameState
{
public:
	virtual void init(sf::RenderWindow& rWindow) = 0;
	virtual void exit() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
};