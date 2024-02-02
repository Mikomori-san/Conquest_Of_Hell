#include "stdafx.h"
#include "InputManager.h"
#include "../Enums/GamepadButton.h"
#include <SFML/System.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

void InputManager::init(sf::Window& window)
{
	window.setKeyRepeatEnabled(false);
	
	isKeyDown[sf::Keyboard::Key::W] = false;
	isKeyDown[sf::Keyboard::Key::A] = false;
	isKeyDown[sf::Keyboard::Key::S] = false;
	isKeyDown[sf::Keyboard::Key::D] = false;
	isKeyDown[sf::Keyboard::Key::Space] = false;
	isKeyDown[sf::Keyboard::Key::Num1] = false;
	isKeyDown[sf::Keyboard::Key::Num2] = false;
	isKeyDown[sf::Keyboard::Key::Up] = false;
	isKeyDown[sf::Keyboard::Key::Left] = false;
	isKeyDown[sf::Keyboard::Key::Down] = false;
	isKeyDown[sf::Keyboard::Key::Right] = false;
	isKeyDown[sf::Keyboard::Key::Home] = false;
	isKeyDown[sf::Keyboard::Key::End] = false;
	isKeyDown[sf::Keyboard::Key::PageUp] = false;
	isKeyDown[sf::Keyboard::Key::PageDown] = false;
	isKeyDown[sf::Keyboard::Enter] = false;
	isKeyDown[sf::Keyboard::Q] = false;
	isKeyDown[sf::Keyboard::E] = false;
	isKeyDown[sf::Keyboard::LAlt] = false;
	isKeyDown[sf::Keyboard::RControl] = false;

	isKeyUp[sf::Keyboard::Key::W] = false;
	isKeyUp[sf::Keyboard::Key::A] = false;
	isKeyUp[sf::Keyboard::Key::S] = false;
	isKeyUp[sf::Keyboard::Key::D] = false;
	isKeyUp[sf::Keyboard::Key::Space] = false;
	isKeyUp[sf::Keyboard::Key::Num1] = false;
	isKeyUp[sf::Keyboard::Key::Num2] = false;
	isKeyUp[sf::Keyboard::Key::Up] = false;
	isKeyUp[sf::Keyboard::Key::Left] = false;
	isKeyUp[sf::Keyboard::Key::Down] = false;
	isKeyUp[sf::Keyboard::Key::Right] = false;
	isKeyUp[sf::Keyboard::Key::Home] = false;
	isKeyUp[sf::Keyboard::Key::End] = false;
	isKeyUp[sf::Keyboard::Key::PageUp] = false;
	isKeyUp[sf::Keyboard::Key::PageDown] = false;
	isKeyUp[sf::Keyboard::Enter] = false;
	isKeyUp[sf::Keyboard::Q] = false;
	isKeyUp[sf::Keyboard::E] = false;
	isKeyUp[sf::Keyboard::LAlt] = false;
	isKeyUp[sf::Keyboard::RControl] = false;

	isKeyPressed[sf::Keyboard::Key::W] = false;
	isKeyPressed[sf::Keyboard::Key::A] = false;
	isKeyPressed[sf::Keyboard::Key::S] = false;
	isKeyPressed[sf::Keyboard::Key::D] = false;
	isKeyPressed[sf::Keyboard::Key::Space] = false;
	isKeyPressed[sf::Keyboard::Key::Num1] = false;
	isKeyPressed[sf::Keyboard::Key::Num2] = false;
	isKeyPressed[sf::Keyboard::Key::Up] = false;
	isKeyPressed[sf::Keyboard::Key::Left] = false;
	isKeyPressed[sf::Keyboard::Key::Down] = false;
	isKeyPressed[sf::Keyboard::Key::Right] = false;
	isKeyPressed[sf::Keyboard::Key::Home] = false;
	isKeyPressed[sf::Keyboard::Key::End] = false;
	isKeyPressed[sf::Keyboard::Key::PageUp] = false;
	isKeyPressed[sf::Keyboard::Key::PageDown] = false;
	isKeyPressed[sf::Keyboard::Enter] = false;
	isKeyPressed[sf::Keyboard::Q] = false;
	isKeyPressed[sf::Keyboard::E] = false;
	isKeyPressed[sf::Keyboard::LAlt] = false;
	isKeyPressed[sf::Keyboard::RControl] = false;

	for (sf::Mouse::Button button = sf::Mouse::Left; button <= sf::Mouse::XButton2; button = static_cast<sf::Mouse::Button>(static_cast<int>(button) + 1)) {
		isMouseDown[button] = false;
		isMouseUp[button] = false;
		isMousePressed[button] = false;
	}

	mousePosition = sf::Vector2i(0, 0);
}

void InputManager::update()
{

	for(auto iteration = isKeyDown.begin(); iteration != isKeyDown.end(); ++iteration)
	{
		isKeyDown[iteration->first] = false;
	}

	for (auto iteration = isKeyUp.begin(); iteration != isKeyUp.end(); ++iteration)
	{
		isKeyUp[iteration->first] = false;
	}

	for (auto& pair : isMouseDown) {
		pair.second = false;
	}
	for (auto& pair : isMouseUp) {
		pair.second = false;
	}
}

bool InputManager::getKeyPressed(sf::Keyboard::Key key)
{
	return isKeyPressed[key] ? isKeyPressed[key] : false;
}

bool InputManager::getKeyDown(sf::Keyboard::Key key)
{
	return isKeyDown[key] ? isKeyDown[key] : false;
}

bool InputManager::getKeyUp(sf::Keyboard::Key key)
{
	return isKeyUp[key] ? isKeyUp[key] : false;
}

void InputManager::handleEvents(sf::Event& event)
{
	if (event.type == sf::Event::EventType::KeyPressed)
	{
		isKeyDown[event.key.code] = true;
		isKeyPressed[event.key.code] = true;
	}
	else if (event.type == sf::Event::EventType::KeyReleased)
	{
		isKeyUp[event.key.code] = true;
		isKeyPressed[event.key.code] = false;
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		isMouseDown[event.mouseButton.button] = true;
		isMousePressed[event.mouseButton.button] = true;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		isMouseUp[event.mouseButton.button] = true;
		isMousePressed[event.mouseButton.button] = false;
	}
	else if (event.type == sf::Event::Resized)
	{
		mousePosition = sf::Mouse::getPosition(*m_window);
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		mousePosition = sf::Mouse::getPosition(*m_window);
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
	return isMousePressed[button];
}

bool InputManager::getMouseDown(sf::Mouse::Button button)
{
	return isMouseDown[button];
}

bool InputManager::getMouseUp(sf::Mouse::Button button)
{
	return isMouseUp[button];
}

sf::Vector2i InputManager::getMousePosition()
{
	return mousePosition;
}

bool InputManager::checkGamepadInput(GamepadButton button, int controllerNr)
{
	return sf::Joystick::isButtonPressed(controllerNr, button);
}