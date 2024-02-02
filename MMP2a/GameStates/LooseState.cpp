#include "stdafx.h"
#include "LooseState.h"
#include "../Components/Graphics_Components/AnimatedGraphicsCP.h"
#include "../Components/Graphics_Components/StandardGraphicsCP.h"
#include "../Components/Transformation_Components/TransformationCP.h"
#include "../Components/Transformation_Components/BackgroundTransformationCP.h"
#include "../Components/Input_Components/MovementInputWASDCP.h"
#include "../Components/Input_Components/MovementInputArrowsCP.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Render_Components/RenderCP.h"
#include "../Components/CameraCP.h"
#include "../Components/Transformation_Components/CameraTransformationCP.h"
#include "../Components/Render_Components/SpriteRenderCP.h"

#include "../Manager/GameStateManager.h"


#include <SFML/Graphics.hpp>
#include <iostream>


void LooseState::init(sf::RenderWindow& rWindow)
{
	this->window.reset(&rWindow, [](sf::RenderWindow*) {});

	DebugDraw::getInstance().initialize(*window);

	AssetManager::getInstance().loadMusic("Celestial_Wanderer", "Assets\\Music\\Celestial_Wanderer.mp3");
	cw = AssetManager::getInstance().Music["Celestial_Wanderer"];
	cw->setVolume(10);
	cw->play();

	window->setView(sf::View(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), (sf::Vector2f)window->getSize()));

	AssetManager::getInstance().loadTexture("BackgroundImage", "Assets\\Textures\\overScreen.png");


	if (!AssetManager::getInstance().Textures["BackgroundImage"])
	{
		std::cout << "Background-Image not found " << std::endl;
	}


	bgTexture = *AssetManager::getInstance().Textures["BackgroundImage"];
	backgroundSprite.setTexture(bgTexture);

	sf::Vector2f viewCenterVec = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2));
	sf::Vector2f startButtonPos = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2) + 50);
	sf::Vector2f exitButtonPos = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2) + 150);
	backgroundSprite.setOrigin(sf::Vector2(backgroundSprite.getGlobalBounds().width / 2, backgroundSprite.getGlobalBounds().height / 2));
	backgroundSprite.setPosition(viewCenterVec);






	close = false;
}

void LooseState::exit()
{
	cw->stop();
	AssetManager::getInstance().unloadAssets();
	DebugDraw::getInstance().unload();
}

void LooseState::update(float deltaTime)
{

}

void LooseState::render()
{

	window->clear(sf::Color::Black);
	window->draw(backgroundSprite);

}

