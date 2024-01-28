#include "stdafx.h"
#include <iostream>
#include <SFML/Window.hpp>
#include "Manager/AssetManager.h"
#include "Game.h"

int main()
{
	std::cout << "Press ESC to close the window" << std::endl;

	Game game;
	game.run();

	std::cout << "All done" << std::endl;
}