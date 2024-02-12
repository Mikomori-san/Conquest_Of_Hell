//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "Game.h"
#include "Manager/AssetManager.h"
#include <SFML/Window.hpp>
#include <iostream>


int main()
{
	std::cout << "Press ESC to close the window" << std::endl;

	Game game;
	game.run();

	std::cout << "All done" << std::endl;
}