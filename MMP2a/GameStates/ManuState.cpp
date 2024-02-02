#include "stdafx.h"
#include "MenuState.h"
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


void MenuState::init(sf::RenderWindow& rWindow)
{
	this->window.reset(&rWindow, [](sf::RenderWindow*) {});	

	//this->window->setSize(sf::Vector2u(960, 540));

	DebugDraw::getInstance().initialize(*window);

	window->setView(sf::View(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), (sf::Vector2f)window->getSize()));

	AssetManager::getInstance().loadTexture("BackgroundImage", "Assets\\Textures\\TitleScreenBackgroundTitle.png");
	AssetManager::getInstance().loadTexture("startImage", "Assets\\Textures\\start.png");
	AssetManager::getInstance().loadTexture("exitImage", "Assets\\Textures\\exit.png");
	AssetManager::getInstance().loadFont("standardFont", "Assets\\Fonts\\Perfect.ttf");
	
	if (!AssetManager::getInstance().Textures["BackgroundImage"])
	{
		std::cout << "Background-Image not found " << std::endl;
	}
	else if (!AssetManager::getInstance().Textures["startImage"])
	{
		std::cout << "Start-Image not found " << std::endl;
	}
	else if (!AssetManager::getInstance().Textures["exitImage"])
	{
		std::cout << "Exit-Image not found " << std::endl;
	}

	bgTexture = *AssetManager::getInstance().Textures["BackgroundImage"];
	backgroundSprite.setTexture(bgTexture);

	sf::Vector2f viewCenterVec = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2));
	sf::Vector2f startButtonPos = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2) + 50);
	sf::Vector2f exitButtonPos = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2) + 150);
	backgroundSprite.setOrigin(sf::Vector2(backgroundSprite.getGlobalBounds().width / 2, backgroundSprite.getGlobalBounds().height / 2));
	backgroundSprite.setPosition(viewCenterVec);

	startTexture = *AssetManager::getInstance().Textures["startImage"];
	startSprite.setTexture(startTexture);
	startSprite.setOrigin(sf::Vector2(startSprite.getGlobalBounds().width / 2, startSprite.getGlobalBounds().height / 2));
	startSprite.setPosition(startButtonPos);
	startSprite.setScale(0.5f, 0.5f);

	exitTexture = *AssetManager::getInstance().Textures["exitImage"];
	exitSprite.setTexture(exitTexture);
	exitSprite.setOrigin(sf::Vector2(exitSprite.getGlobalBounds().width / 2, exitSprite.getGlobalBounds().height / 2));
	exitSprite.setPosition(exitButtonPos);
	exitSprite.setScale(0.5f, 0.5f);
	
	sf::Color red = sf::Color(255, 0, 0, 255);
	sf::Color black = sf::Color(0, 0, 0);
	sf::Color white = sf::Color(255, 255, 255, 255 );

	sf::Font font = *AssetManager::getInstance().Fonts["standardFont"];

	/*start = createText("Start", font, 24, startButtonPos, red);
	exitText = createText("Exit", font, 24, exitButtonPos, red);*/

	createButton("Start", sf::Vector2f(750, 50), startButtonPos, black);
	createButton("Exit", sf::Vector2f(750, 50), exitButtonPos, red);

}

void MenuState::exit()
{
	AssetManager::getInstance().unloadAssets();
	DebugDraw::getInstance().unload();
}

void MenuState::update(float deltaTime)
{
	for (auto& button : buttonMap)
	{
		sf::FloatRect buttonBounds = button.second.getGlobalBounds();
		sf::Vector2i mousePos = InputManager::getInstance().getMousePosition();

		if (buttonBounds.contains(static_cast<sf::Vector2f>(mousePos)))
		{
			button.second.setOutlineColor(sf::Color::White);
			button.second.setOutlineThickness(1.f);


			if (button.first == "Start" && InputManager::getInstance().getMouseUp(sf::Mouse::Left))
			{
				std::cout << "start pressed" << std::endl;
				GameStateManager::getInstance().setState("Gameplay", *window);

			}
			if (button.first == "Exit" && InputManager::getInstance().getMouseUp(sf::Mouse::Left))
			{
				std::cout << "Exit pressed" << std::endl;
				GameStateManager::getInstance().setState("Exit", *window);

			}
	
		}
		else
		{
			button.second.setFillColor(sf::Color::Black);
			button.second.setOutlineColor(sf::Color::Red);
			button.second.setOutlineThickness(1.f);
		}
	}
}

void MenuState::render()
{
	
	window->clear(sf::Color::Black);
	window->draw(backgroundSprite);
	//window->draw(start);
	//window->draw(exitText);
	//window->draw(startButton);
	//window->draw(exitButton);

	for (auto& pair : buttonMap) {
		window->draw(pair.second);
		
	}

	window->draw(startSprite);
	window->draw(exitSprite);
}

//sf::Text MenuState::createText(std::string text, sf::Font font,int charSize, sf::Vector2f pos, sf::Color fillColor)
//{
//	sf::Text words;
//	words.setFont(font);
//	words.setString(text);
//	words.setCharacterSize(charSize);
//	words.setFillColor(fillColor);
//	words.setPosition(pos);
//	
//	return words;
//}
// sf::RectangleShape MenuState::createButton( sf::Text name, sf::Vector2f size, sf::Vector2f pos, sf::Color fillColor)
//{
//	sf::RectangleShape button;
//	button.setSize(size);
//	button.setOrigin(sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2));
//	button.setFillColor(fillColor);
//	button.setPosition(pos);
//
//	buttonList.push_back(std::make_pair(name, button));
//
//	return button;
//}
 sf::RectangleShape MenuState::createButton( std::string name, sf::Vector2f size, sf::Vector2f pos, sf::Color fillColor)
{
	sf::RectangleShape button;
	button.setSize(size);
	button.setOrigin(sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2));
	button.setFillColor(fillColor);
	button.setPosition(pos);

	buttonMap[name] = button;

	return button;
}