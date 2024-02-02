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

	std::map<sf::Keyboard::Key, bool> isKeyDown;
	std::map<sf::Keyboard::Key, bool> isKeyPressed;
	std::map<sf::Keyboard::Key, bool> isKeyUp;

	std::map<sf::Mouse::Button, bool> isMouseDown;
	std::map<sf::Mouse::Button, bool> isMouseUp;
	std::map<sf::Mouse::Button, bool> isMousePressed;

	sf::Vector2i mousePosition;

	sf::RenderWindow* m_window;


};