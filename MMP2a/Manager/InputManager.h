#pragma once

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

private:

	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	std::map<sf::Keyboard::Key, bool> isKeyDown;
	std::map<sf::Keyboard::Key, bool> isKeyPressed;
	std::map<sf::Keyboard::Key, bool> isKeyUp;
};