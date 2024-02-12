//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "MenuState.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Graphics_Components/AnimatedGraphicsCP.h"
#include "../Components/Input_Components/MovementInputWASDCP.h"
#include "../Components/Render_Components/RenderCP.h"
#include "../Components/Render_Components/SpriteRenderCP.h"
#include "../Enums/GamepadButton.h"
#include "../Manager/GameStateManager.h"
#include "MenuState.h"
#include <SFML/Graphics.hpp>
#include <iostream>



void MenuState::init(sf::RenderWindow& rWindow)
{
	this->m_window.reset(&rWindow, [](sf::RenderWindow*) {});	

	DebugDraw::getInstance().initialize(*m_window);
	
	AssetManager::getInstance().loadMusic("Celestial_Wanderer", "Assets\\Music\\Celestial_Wanderer.mp3");
	m_cw = AssetManager::getInstance().Music["Celestial_Wanderer"];
	m_cw->setVolume(3);
	m_cw->setLoop(true);
	m_cw->play();
	
	m_window->setView(sf::View(sf::Vector2f(m_window->getSize().x / 2, m_window->getSize().y / 2), (sf::Vector2f)m_window->getSize()));

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

	m_backgroundSprite = sf::Sprite(*AssetManager::getInstance().Textures["BackgroundImage"]);

	sf::Vector2f viewCenterVec = sf::Vector2f((m_window->getSize().x / 2), (m_window->getSize().y / 2));
	sf::Vector2f startButtonPos = sf::Vector2f((m_window->getSize().x / 2), (m_window->getSize().y / 2) + 50);
	sf::Vector2f exitButtonPos = sf::Vector2f((m_window->getSize().x / 2), (m_window->getSize().y / 2) + 150);
	m_backgroundSprite.setOrigin(sf::Vector2(m_backgroundSprite.getGlobalBounds().width / 2, m_backgroundSprite.getGlobalBounds().height / 2));
	m_backgroundSprite.setPosition(viewCenterVec);


	m_startSprite = sf::Sprite(*AssetManager::getInstance().Textures["startImage"]);
	m_startSprite.setOrigin(sf::Vector2(m_startSprite.getGlobalBounds().width / 2, m_startSprite.getGlobalBounds().height / 2));
	m_startSprite.setPosition(startButtonPos);
	m_startSprite.setScale(0.5f, 0.5f);

	m_exitSprite = sf::Sprite(*AssetManager::getInstance().Textures["exitImage"]);
	m_exitSprite.setOrigin(sf::Vector2(m_exitSprite.getGlobalBounds().width / 2, m_exitSprite.getGlobalBounds().height / 2));
	m_exitSprite.setPosition(exitButtonPos);
	m_exitSprite.setScale(0.5f, 0.5f);
	
	m_gamePadSprite = sf::Sprite(*AssetManager::getInstance().Textures["gamePad"]);
	m_gamePadSprite.setOrigin(sf::Vector2(m_gamePadSprite.getGlobalBounds().width / 2, m_gamePadSprite.getGlobalBounds().height / 2));
	m_gamePadSprite.setPosition(sf::Vector2f(1800.f, 100.f));
	//gamePadSprite.setScale(0.5f, 0.5f);


	sf::Color red = sf::Color(255, 0, 0, 255);
	sf::Color black = sf::Color(0, 0, 0);
	sf::Color white = sf::Color(255, 255, 255, 255 );


	createButton("Start", sf::Vector2f(750, 50), startButtonPos, black);
	createButton("Exit", sf::Vector2f(750, 50), exitButtonPos, black);

	m_close = false;

	sf::Font font;

	if (!AssetManager::getInstance().Fonts["perfect"]) {
		AssetManager::getInstance().loadFont("perfect", "Assets\\Fonts\\Perfect.ttf");
	}

	font = *AssetManager::getInstance().Fonts["perfect"];

	if (font.getInfo().family.empty()) {
		std::cerr << "Font not loaded properly!" << std::endl;
	}

	m_text = sf::Text();
	m_text.setFont(font);
	m_text.setString("Press Start to enable Gamepad!");
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::White);

	sf::FloatRect textBounds = m_text.getLocalBounds();
	m_text.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	m_text.setPosition(m_window->getSize().x / 2.0f, m_window->getSize().y / 2.0f);
	m_textTimer = 0.f;
}

void MenuState::exit()
{
	m_cw->stop();
	AssetManager::getInstance().unloadAssets();
	DebugDraw::getInstance().unload();
}

void MenuState::update(float deltaTime)
{
	m_buttonTimer += deltaTime;
	m_textTimer += deltaTime;

	if (sf::Joystick::isButtonPressed(0, GamepadButton::Start) && m_buttonTimer >= CONTROLLER_SWITCH_CD)
	{
		GameStateManager::getInstance().toggleGamepadUse();
		m_buttonTimer = 0.f;
	}

	if (!sf::Joystick::isConnected(0))
	{
		if (GameStateManager::getInstance().gamepadUse())
			GameStateManager::getInstance().toggleGamepadUse();
	}

	if (GameStateManager::getInstance().gamepadUse())
	{
		m_gamePadSprite = sf::Sprite(*AssetManager::getInstance().Textures["gamePad"]);
	}
	else
	{
		m_gamePadSprite = sf::Sprite(*AssetManager::getInstance().Textures["noGamePad"]);
	}

	for (auto& button : m_buttonMap)
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
				GameStateManager::getInstance().setState("Gameplay", *m_window);
			}
			if (button.first == "Exit" && InputManager::getInstance().getMouseUp(sf::Mouse::Left))
			{
				m_window->close();
			}
		}
		else if (GameStateManager::getInstance().gamepadUse())
		{
			sf::Vector2f joyStickLocation = InputManager::getInstance().getLeftStickPosition(0);
			if (m_isPlaySelected)
			{
				if (button.first == "Start")
				{
					button.second.setOutlineColor(sf::Color::White);
					if (sf::Joystick::isButtonPressed(0, GamepadButton::A))
					{
						GameStateManager::getInstance().setState("Gameplay", *m_window);
					}
				}
			}
			
			if (joyStickLocation.y >= 0.2f)
			{
				m_isPlaySelected = false;
				m_isExitSelected = true;

			}
			else if (joyStickLocation.y <= -0.2f)
			{
				m_isPlaySelected = true;
				m_isExitSelected = false;
			}
			if (m_isExitSelected)
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
					m_window->close();

				}
			}
		}
	}
}

void MenuState::render()
{
	m_window->clear(sf::Color::Black);
	m_window->draw(m_backgroundSprite);

	for (auto& pair : m_buttonMap) {
		m_window->draw(pair.second);
		
	}

	int intTextTimer = std::round(m_textTimer);

	//BECAUSE OF SFML BUG: (Doesn't work "clean")
	sf::Font font;

	if (!AssetManager::getInstance().Fonts["perfect"]) {
		AssetManager::getInstance().loadFont("perfect", "Assets\\Fonts\\Perfect.ttf");
	}

	font = *AssetManager::getInstance().Fonts["perfect"];

	if (font.getInfo().family.empty()) {
		std::cerr << "Font not loaded properly!" << std::endl;
	}
	
	m_text.setFont(font);
		
	m_window->draw(m_startSprite);
	m_window->draw(m_exitSprite);
	m_window->draw(m_gamePadSprite);
	
	if (intTextTimer % 2 == 0)
	{
		if (!GameStateManager::getInstance().gamepadUse())
		{
			m_text.setString("Press Start to enable Gamepad!");
			sf::FloatRect textBounds = m_text.getLocalBounds();
			m_text.setOrigin(textBounds.left + textBounds.width / 2.0f,
				textBounds.top + textBounds.height / 2.0f);
			m_text.setPosition(m_window->getSize().x / 2.0f, m_window->getSize().y / 2.0f);
		}
		else
		{
			m_text.setString("Gamepad connected!");
			sf::FloatRect textBounds = m_text.getLocalBounds();
			m_text.setOrigin(textBounds.left + textBounds.width / 2.0f,
				textBounds.top + textBounds.height / 2.0f);
			m_text.setPosition(m_window->getSize().x / 2.0f, m_window->getSize().y / 2.0f);
		}
			
		m_window->draw(m_text);
	}
}
 sf::RectangleShape MenuState::createButton( std::string name, sf::Vector2f size, sf::Vector2f pos, sf::Color fillColor)
{
	sf::RectangleShape button;
	button.setSize(size);
	button.setOrigin(sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2));
	button.setFillColor(fillColor);
	button.setPosition(pos);

	m_buttonMap[name] = button;

	return button;
}