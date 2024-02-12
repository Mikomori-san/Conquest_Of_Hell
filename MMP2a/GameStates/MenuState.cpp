//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "MenuState.h"
#include "../Components/CameraCP.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Graphics_Components/AnimatedGraphicsCP.h"
#include "../Components/Graphics_Components/StandardGraphicsCP.h"
#include "../Components/Input_Components/MovementInputWASDCP.h"
#include "../Components/Render_Components/RenderCP.h"
#include "../Components/Render_Components/SpriteRenderCP.h"
#include "../Components/Transformation_Components/BackgroundTransformationCP.h"
#include "../Components/Transformation_Components/CameraTransformationCP.h"
#include "../Components/Transformation_Components/TransformationCP.h"
#include "../Enums/GamepadButton.h"
#include "../Manager/GameStateManager.h"
#include "MenuState.h"
#include <SFML/Graphics.hpp>
#include <iostream>



void MenuState::init(sf::RenderWindow& rWindow)
{
	this->window.reset(&rWindow, [](sf::RenderWindow*) {});	

	DebugDraw::getInstance().initialize(*window);
	
	AssetManager::getInstance().loadMusic("Celestial_Wanderer", "Assets\\Music\\Celestial_Wanderer.mp3");
	cw = AssetManager::getInstance().Music["Celestial_Wanderer"];
	cw->setVolume(3);
	cw->setLoop(true);
	cw->play();
	
	window->setView(sf::View(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), (sf::Vector2f)window->getSize()));

	AssetManager::getInstance().loadTexture("BackgroundImage", "Assets\\Textures\\TitleScreenBackgroundTitle.png");
	AssetManager::getInstance().loadTexture("startImage", "Assets\\Textures\\start.png");
	AssetManager::getInstance().loadTexture("exitImage", "Assets\\Textures\\exit.png");
	AssetManager::getInstance().loadTexture("gamePad", "Assets\\Textures\\GamePad.png");
	AssetManager::getInstance().loadTexture("noGamePad", "Assets\\Textures\\noGamePad.png");
	
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

	backgroundSprite = sf::Sprite(*AssetManager::getInstance().Textures["BackgroundImage"]);

	sf::Vector2f viewCenterVec = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2));
	sf::Vector2f startButtonPos = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2) + 50);
	sf::Vector2f exitButtonPos = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2) + 150);
	backgroundSprite.setOrigin(sf::Vector2(backgroundSprite.getGlobalBounds().width / 2, backgroundSprite.getGlobalBounds().height / 2));
	backgroundSprite.setPosition(viewCenterVec);


	startSprite = sf::Sprite(*AssetManager::getInstance().Textures["startImage"]);
	startSprite.setOrigin(sf::Vector2(startSprite.getGlobalBounds().width / 2, startSprite.getGlobalBounds().height / 2));
	startSprite.setPosition(startButtonPos);
	startSprite.setScale(0.5f, 0.5f);

	exitSprite = sf::Sprite(*AssetManager::getInstance().Textures["exitImage"]);
	exitSprite.setOrigin(sf::Vector2(exitSprite.getGlobalBounds().width / 2, exitSprite.getGlobalBounds().height / 2));
	exitSprite.setPosition(exitButtonPos);
	exitSprite.setScale(0.5f, 0.5f);
	
	gamePadSprite = sf::Sprite(*AssetManager::getInstance().Textures["gamePad"]);
	gamePadSprite.setOrigin(sf::Vector2(gamePadSprite.getGlobalBounds().width / 2, gamePadSprite.getGlobalBounds().height / 2));
	gamePadSprite.setPosition(sf::Vector2f(1800.f, 100.f));
	//gamePadSprite.setScale(0.5f, 0.5f);


	sf::Color red = sf::Color(255, 0, 0, 255);
	sf::Color black = sf::Color(0, 0, 0);
	sf::Color white = sf::Color(255, 255, 255, 255 );


	createButton("Start", sf::Vector2f(750, 50), startButtonPos, black);
	createButton("Exit", sf::Vector2f(750, 50), exitButtonPos, black);

	close = false;

	sf::Font font;

	if (!AssetManager::getInstance().Fonts["perfect"]) {
		AssetManager::getInstance().loadFont("perfect", "Assets\\Fonts\\Perfect.ttf");
	}

	font = *AssetManager::getInstance().Fonts["perfect"];

	if (font.getInfo().family.empty()) {
		std::cerr << "Font not loaded properly!" << std::endl;
	}

	text = sf::Text();
	text.setFont(font);
	text.setString("Press Start to enable Gamepad!");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	text.setPosition(window->getSize().x / 2.0f, window->getSize().y / 2.0f);
	textTimer = 0.f;
}

void MenuState::exit()
{
	cw->stop();
	AssetManager::getInstance().unloadAssets();
	DebugDraw::getInstance().unload();
}

void MenuState::update(float deltaTime)
{
	buttonTimer += deltaTime;
	textTimer += deltaTime;

	if (sf::Joystick::isButtonPressed(0, GamepadButton::Start) && buttonTimer >= CONTROLLER_SWITCH_CD)
	{
		GameStateManager::getInstance().toggleGamepadUse();
		buttonTimer = 0.f;
	}

	if (!sf::Joystick::isConnected(0))
	{
		if (GameStateManager::getInstance().gamepadUse())
			GameStateManager::getInstance().toggleGamepadUse();
	}

	if (GameStateManager::getInstance().gamepadUse())
	{
		gamePadSprite = sf::Sprite(*AssetManager::getInstance().Textures["gamePad"]);
	}
	else
	{
		gamePadSprite = sf::Sprite(*AssetManager::getInstance().Textures["noGamePad"]);
	}

	for (auto& button : buttonMap)
	{
		sf::FloatRect buttonBounds = button.second.getGlobalBounds();
		sf::Vector2i mousePos = InputManager::getInstance().getMousePosition();

		button.second.setFillColor(sf::Color::Black);
		button.second.setOutlineColor(sf::Color::Red);
		button.second.setOutlineThickness(1.f);

		if (buttonBounds.contains(static_cast<sf::Vector2f>(mousePos)))
		{
			button.second.setOutlineColor(sf::Color::White);

			if (button.first == "Start" && InputManager::getInstance().getMouseUp(sf::Mouse::Left))
			{
				GameStateManager::getInstance().setState("Gameplay", *window);
			}
			if (button.first == "Exit" && InputManager::getInstance().getMouseUp(sf::Mouse::Left))
			{
				window->close();
			}
		}
		else if (GameStateManager::getInstance().gamepadUse())
		{
			sf::Vector2f joyStickLocation = InputManager::getInstance().getLeftStickPosition(0);
			if (isPlaySelected)
			{
				if (button.first == "Start")
				{
					button.second.setOutlineColor(sf::Color::White);
					if (sf::Joystick::isButtonPressed(0, GamepadButton::A))
					{
						GameStateManager::getInstance().setState("Gameplay", *window);
					}
				}
			}
			
			if (joyStickLocation.y >= 0.2f)
			{
				isPlaySelected = false;
				isExitSelected = true;

			}
			else if (joyStickLocation.y <= -0.2f)
			{
				isPlaySelected = true;
				isExitSelected = false;
			}
			if (isExitSelected)
			{
				if (button.first == "Start")
				{
					button.second.setOutlineColor(sf::Color::Red);
				}
				else
				{
					button.second.setOutlineColor(sf::Color::White);
				}
				if (sf::Joystick::isButtonPressed(0, static_cast<GamepadButton>(A)))
				{
					window->close();

				}
			}
		}
	}
}

void MenuState::render()
{
	window->clear(sf::Color::Black);
	window->draw(backgroundSprite);

	for (auto& pair : buttonMap) {
		window->draw(pair.second);
		
	}

	int intTextTimer = std::round(textTimer);

	//BECAUSE OF SFML BUG: (Doesn't work "clean")
	sf::Font font;

	if (!AssetManager::getInstance().Fonts["perfect"]) {
		AssetManager::getInstance().loadFont("perfect", "Assets\\Fonts\\Perfect.ttf");
	}

	font = *AssetManager::getInstance().Fonts["perfect"];

	if (font.getInfo().family.empty()) {
		std::cerr << "Font not loaded properly!" << std::endl;
	}
	
	text.setFont(font);
		
	window->draw(startSprite);
	window->draw(exitSprite);
	window->draw(gamePadSprite);
	
	if (intTextTimer % 2 == 0)
	{
		if (!GameStateManager::getInstance().gamepadUse())
		{
			text.setString("Press Start to enable Gamepad!");
			sf::FloatRect textBounds = text.getLocalBounds();
			text.setOrigin(textBounds.left + textBounds.width / 2.0f,
				textBounds.top + textBounds.height / 2.0f);
			text.setPosition(window->getSize().x / 2.0f, window->getSize().y / 2.0f);
		}
		else
		{
			text.setString("Gamepad connected!");
			sf::FloatRect textBounds = text.getLocalBounds();
			text.setOrigin(textBounds.left + textBounds.width / 2.0f,
				textBounds.top + textBounds.height / 2.0f);
			text.setPosition(window->getSize().x / 2.0f, window->getSize().y / 2.0f);
		}
			
		window->draw(text);
	}
}
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