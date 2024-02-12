//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "InputManager.h"
#include "../Enums/GamepadButton.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <map>

void InputManager::init(sf::Window& window)
{
	window.setKeyRepeatEnabled(false);
	
	m_isKeyDown[sf::Keyboard::Key::W] = false;
	m_isKeyDown[sf::Keyboard::Key::A] = false;
	m_isKeyDown[sf::Keyboard::Key::S] = false;
	m_isKeyDown[sf::Keyboard::Key::D] = false;
	m_isKeyDown[sf::Keyboard::Key::Space] = false;
	m_isKeyDown[sf::Keyboard::Key::Num1] = false;
	m_isKeyDown[sf::Keyboard::Key::Num2] = false;
	m_isKeyDown[sf::Keyboard::Key::Up] = false;
	m_isKeyDown[sf::Keyboard::Key::Left] = false;
	m_isKeyDown[sf::Keyboard::Key::Down] = false;
	m_isKeyDown[sf::Keyboard::Key::Right] = false;
	m_isKeyDown[sf::Keyboard::Key::Home] = false;
	m_isKeyDown[sf::Keyboard::Key::End] = false;
	m_isKeyDown[sf::Keyboard::Key::PageUp] = false;
	m_isKeyDown[sf::Keyboard::Key::PageDown] = false;
	m_isKeyDown[sf::Keyboard::Enter] = false;
	m_isKeyDown[sf::Keyboard::Q] = false;
	m_isKeyDown[sf::Keyboard::E] = false;
	m_isKeyDown[sf::Keyboard::LAlt] = false;
	m_isKeyDown[sf::Keyboard::RControl] = false;

	m_isKeyUp[sf::Keyboard::Key::W] = false;
	m_isKeyUp[sf::Keyboard::Key::A] = false;
	m_isKeyUp[sf::Keyboard::Key::S] = false;
	m_isKeyUp[sf::Keyboard::Key::D] = false;
	m_isKeyUp[sf::Keyboard::Key::Space] = false;
	m_isKeyUp[sf::Keyboard::Key::Num1] = false;
	m_isKeyUp[sf::Keyboard::Key::Num2] = false;
	m_isKeyUp[sf::Keyboard::Key::Up] = false;
	m_isKeyUp[sf::Keyboard::Key::Left] = false;
	m_isKeyUp[sf::Keyboard::Key::Down] = false;
	m_isKeyUp[sf::Keyboard::Key::Right] = false;
	m_isKeyUp[sf::Keyboard::Key::Home] = false;
	m_isKeyUp[sf::Keyboard::Key::End] = false;
	m_isKeyUp[sf::Keyboard::Key::PageUp] = false;
	m_isKeyUp[sf::Keyboard::Key::PageDown] = false;
	m_isKeyUp[sf::Keyboard::Enter] = false;
	m_isKeyUp[sf::Keyboard::Q] = false;
	m_isKeyUp[sf::Keyboard::E] = false;
	m_isKeyUp[sf::Keyboard::LAlt] = false;
	m_isKeyUp[sf::Keyboard::RControl] = false;

	m_isKeyPressed[sf::Keyboard::Key::W] = false;
	m_isKeyPressed[sf::Keyboard::Key::A] = false;
	m_isKeyPressed[sf::Keyboard::Key::S] = false;
	m_isKeyPressed[sf::Keyboard::Key::D] = false;
	m_isKeyPressed[sf::Keyboard::Key::Space] = false;
	m_isKeyPressed[sf::Keyboard::Key::Num1] = false;
	m_isKeyPressed[sf::Keyboard::Key::Num2] = false;
	m_isKeyPressed[sf::Keyboard::Key::Up] = false;
	m_isKeyPressed[sf::Keyboard::Key::Left] = false;
	m_isKeyPressed[sf::Keyboard::Key::Down] = false;
	m_isKeyPressed[sf::Keyboard::Key::Right] = false;
	m_isKeyPressed[sf::Keyboard::Key::Home] = false;
	m_isKeyPressed[sf::Keyboard::Key::End] = false;
	m_isKeyPressed[sf::Keyboard::Key::PageUp] = false;
	m_isKeyPressed[sf::Keyboard::Key::PageDown] = false;
	m_isKeyPressed[sf::Keyboard::Enter] = false;
	m_isKeyPressed[sf::Keyboard::Q] = false;
	m_isKeyPressed[sf::Keyboard::E] = false;
	m_isKeyPressed[sf::Keyboard::LAlt] = false;
	m_isKeyPressed[sf::Keyboard::RControl] = false;

	for (sf::Mouse::Button button = sf::Mouse::Left; button <= sf::Mouse::XButton2; button = static_cast<sf::Mouse::Button>(static_cast<int>(button) + 1)) {
		m_isMouseDown[button] = false;
		m_isMouseUp[button] = false;
		m_isMousePressed[button] = false;
	}

	m_mousePosition = sf::Vector2i(0, 0);
}

void InputManager::update()
{

	for(auto iteration = m_isKeyDown.begin(); iteration != m_isKeyDown.end(); ++iteration)
	{
		m_isKeyDown[iteration->first] = false;
	}

	for (auto iteration = m_isKeyUp.begin(); iteration != m_isKeyUp.end(); ++iteration)
	{
		m_isKeyUp[iteration->first] = false;
	}

	for (auto& pair : m_isMouseDown) {
		pair.second = false;
	}
	for (auto& pair : m_isMouseUp) {
		pair.second = false;
	}
}

bool InputManager::getKeyPressed(sf::Keyboard::Key key)
{
	return m_isKeyPressed[key] ? m_isKeyPressed[key] : false;
}

bool InputManager::getKeyDown(sf::Keyboard::Key key)
{
	return m_isKeyDown[key] ? m_isKeyDown[key] : false;
}

bool InputManager::getKeyUp(sf::Keyboard::Key key)
{
	return m_isKeyUp[key] ? m_isKeyUp[key] : false;
}

void InputManager::handleEvents(sf::Event& event)
{
	if (event.type == sf::Event::EventType::KeyPressed)
	{
		m_isKeyDown[event.key.code] = true;
		m_isKeyPressed[event.key.code] = true;
	}
	else if (event.type == sf::Event::EventType::KeyReleased)
	{
		m_isKeyUp[event.key.code] = true;
		m_isKeyPressed[event.key.code] = false;
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		m_isMouseDown[event.mouseButton.button] = true;
		m_isMousePressed[event.mouseButton.button] = true;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		m_isMouseUp[event.mouseButton.button] = true;
		m_isMousePressed[event.mouseButton.button] = false;
	}
	else if (event.type == sf::Event::Resized)
	{
		m_mousePosition = sf::Mouse::getPosition(*m_window);
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		m_mousePosition = sf::Mouse::getPosition(*m_window);
	}
}

sf::Vector2f InputManager::getLeftStickPosition(int gamePadID)
{
	float x = sf::Joystick::getAxisPosition(gamePadID, sf::Joystick::X);
	float y = sf::Joystick::getAxisPosition(gamePadID, sf::Joystick::Y);
	// Normalize die Werte auf den Bereich [-1, 1]
	return sf::Vector2f(x / 100.f, y / 100.f);
}

bool InputManager::getMousePressed(sf::Mouse::Button button)
{
	return m_isMousePressed[button];
}

bool InputManager::getMouseDown(sf::Mouse::Button button)
{
	return m_isMouseDown[button];
}

bool InputManager::getMouseUp(sf::Mouse::Button button)
{
	return m_isMouseUp[button];
}

sf::Vector2i InputManager::getMousePosition()
{
	return m_mousePosition;
}

bool InputManager::checkGamepadInput(GamepadButton button, int controllerNr)
{
	return sf::Joystick::isButtonPressed(controllerNr, button);
}