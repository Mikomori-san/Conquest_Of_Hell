//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Graphics_Components/AnimatedGraphicsCP.h"
#include "../Components/Input_Components/MovementInputWASDCP.h"
#include "../Components/Render_Components/RenderCP.h"
#include "../Components/Render_Components/SpriteRenderCP.h"
#include "../Components/Transformation_Components/TransformationCP.h"
#include "LooseState.h"
#include "../Manager/GameStateManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>


void LooseState::init(sf::RenderWindow& rWindow)
{
	this->m_window.reset(&rWindow, [](sf::RenderWindow*) {});

	DebugDraw::getInstance().initialize(*m_window);

	AssetManager::getInstance().loadMusic("Celestial_Wanderer", "Assets\\Music\\Celestial_Wanderer.mp3");
	m_cw = AssetManager::getInstance().Music["Celestial_Wanderer"];
	m_cw->setVolume(10);
	m_cw->play();

	m_window->setView(sf::View(sf::Vector2f(m_window->getSize().x / 2, m_window->getSize().y / 2), (sf::Vector2f)m_window->getSize()));

	AssetManager::getInstance().loadTexture("BackgroundImage", "Assets\\Textures\\overScreen.png");
	AssetManager::getInstance().loadTexture("restartImage", "Assets\\Textures\\restart.png");
	AssetManager::getInstance().loadTexture("exitImage", "Assets\\Textures\\exit.png");

	if (!AssetManager::getInstance().Textures["BackgroundImage"])
	{
		std::cout << "Background-Image not found " << std::endl;
	}


	m_backgroundSprite = sf::Sprite(*AssetManager::getInstance().Textures["BackgroundImage"]);
	sf::Vector2f viewCenterVec = sf::Vector2f((m_window->getSize().x / 2), (m_window->getSize().y / 2));
	sf::Vector2f startButtonPos = sf::Vector2f((m_window->getSize().x / 2), (m_window->getSize().y / 2) + 180);
	sf::Vector2f exitButtonPos = sf::Vector2f((m_window->getSize().x / 2), (m_window->getSize().y / 2) + 280);
	m_backgroundSprite.setOrigin(sf::Vector2(m_backgroundSprite.getGlobalBounds().width / 2, m_backgroundSprite.getGlobalBounds().height / 2));
	m_backgroundSprite.setPosition(viewCenterVec);

	m_restartSprite = sf::Sprite(*AssetManager::getInstance().Textures["restartImage"]);
	m_restartSprite.setOrigin(sf::Vector2(m_restartSprite.getGlobalBounds().width / 2, m_restartSprite.getGlobalBounds().height / 2));
	m_restartSprite.setPosition(startButtonPos);
	m_restartSprite.setScale(0.5f, 0.5f);

	m_exitSprite = sf::Sprite(*AssetManager::getInstance().Textures["exitImage"]);
	m_exitSprite.setOrigin(sf::Vector2(m_exitSprite.getGlobalBounds().width / 2, m_exitSprite.getGlobalBounds().height / 2));
	m_exitSprite.setPosition(exitButtonPos);
	m_exitSprite.setScale(0.5f, 0.5f);

	sf::Color red = sf::Color(255, 0, 0, 255);
	sf::Color black = sf::Color(0, 0, 0);
	sf::Color white = sf::Color(255, 255, 255, 255);


	createButton("Start", sf::Vector2f(750, 50), startButtonPos, black);
	createButton("Exit", sf::Vector2f(750, 50), exitButtonPos, black);


	m_close = false;
}

void LooseState::exit()
{
	m_cw->stop();
	AssetManager::getInstance().unloadAssets();
	DebugDraw::getInstance().unload();
}

void LooseState::update(float deltaTime)
{
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
		else if (m_isControllerConnected = sf::Joystick::isConnected(0))
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

void LooseState::render()
{


	m_window->clear(sf::Color::Black);
	m_window->draw(m_backgroundSprite);
	for (auto& pair : m_buttonMap) {
		m_window->draw(pair.second);

	}

	m_window->draw(m_restartSprite);
	m_window->draw(m_exitSprite);

}

sf::RectangleShape LooseState::createButton(std::string name, sf::Vector2f size, sf::Vector2f pos, sf::Color fillColor)
{
	sf::RectangleShape button;
	button.setSize(size);
	button.setOrigin(sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2));
	button.setFillColor(fillColor);
	button.setPosition(pos);

	m_buttonMap[name] = button;

	return button;
}