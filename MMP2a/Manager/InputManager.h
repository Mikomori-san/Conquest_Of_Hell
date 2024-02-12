//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Enums/GamepadButton.h"

class InputManager
{
public:
	
	static InputManager& getInstance()
	{
		static InputManager instance;
		return instance;
	}

	void init(sf::Window& window);
	void update();
	bool getKeyPressed(sf::Keyboard::Key key);
	bool getKeyDown(sf::Keyboard::Key key);
	bool getKeyUp(sf::Keyboard::Key key);
	void handleEvents(sf::Event& event);
	bool checkGamepadInput(GamepadButton button, int controllerNr);
	sf::Vector2f getLeftStickPosition(int gamePadID);


	bool getMousePressed(sf::Mouse::Button button);
	bool getMouseDown(sf::Mouse::Button button);
	bool getMouseUp(sf::Mouse::Button button);

	sf::Vector2i getMousePosition();

	void setRenderWindow(sf::RenderWindow& window)
	{
		m_window = &window;
	};

private:

	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	std::map<sf::Keyboard::Key, bool> m_isKeyDown;
	std::map<sf::Keyboard::Key, bool> m_isKeyPressed;
	std::map<sf::Keyboard::Key, bool> m_isKeyUp;

	std::map<sf::Mouse::Button, bool> m_isMouseDown;
	std::map<sf::Mouse::Button, bool> m_isMouseUp;
	std::map<sf::Mouse::Button, bool> m_isMousePressed;

	sf::Vector2i m_mousePosition;

	sf::RenderWindow* m_window;


};