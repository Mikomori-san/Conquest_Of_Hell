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

void MenuState::init(sf::RenderWindow& rWindow)
{
	this->window.reset(&rWindow, [](sf::RenderWindow*) {});

	this->window->setSize(sf::Vector2u(960, 540));

	DebugDraw::getInstance().initialize(*window);

	window->setView(sf::View(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), (sf::Vector2f)window->getSize()));

	AssetManager::getInstance().loadTexture("BackgroundImage", "Assets\\Textures\\TitleScreenBackgroundTitle.png");
	
	if (!AssetManager::getInstance().Textures["BackgroundImage"])
	{
		std::cout << "Background-Image not found " << std::endl;
	}

	bgTexture = *AssetManager::getInstance().Textures["BackgroundImage"];
	backgroundSprite.setTexture(bgTexture);
	backgroundSprite.scale(0.5f, 0.5f);

	sf::Vector2f viewCenterVec = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2));
	sf::Vector2f exitButtonPos = sf::Vector2f((window->getSize().x / 2), (window->getSize().y / 2) + 100);

	sf::Color red = sf::Color( 255, 0, 0, 255 );

	sf::RectangleShape startButton = createButton("Start", sf::Vector2f(250, 50), viewCenterVec, red);
	sf::RectangleShape exitButton = createButton("Exit", sf::Vector2f(250, 50), exitButtonPos, red);

}

void MenuState::exit()
{
	AssetManager::getInstance().unloadAssets();
	DebugDraw::getInstance().unload();
}

void MenuState::update(float deltaTime)
{
	
}

void MenuState::render()
{

	window->clear(sf::Color::Black);
	window->draw(backgroundSprite);


	//window->draw(buttonList["ExitButton"]);
	for (auto& button : buttonList) {
		window->draw(button.second);
	}

}

sf::RectangleShape MenuState::createButton(std::string name, sf::Vector2f size, sf::Vector2f pos, sf::Color fillColor, sf::Color outlineColor)
{
	sf::RectangleShape button;
	button.setSize(size);
	button.setOrigin(sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2));
	button.setFillColor(fillColor);
	button.setOutlineColor(outlineColor);
	button.setPosition(pos);

	buttonList[name] = button;

	return button;
}
sf::RectangleShape MenuState::createButton(std::string name, sf::Vector2f size, sf::Vector2f pos, sf::Color fillColor)
{
	sf::RectangleShape button;
	button.setSize(size);
	button.setOrigin(sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2));
	button.setFillColor(fillColor);
	button.setPosition(pos);

	buttonList[name] = button;

	return button;
}

