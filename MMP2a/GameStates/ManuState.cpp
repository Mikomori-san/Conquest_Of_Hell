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

	this->window->setSize(sf::Vector2u(975, 650));

	DebugDraw::getInstance().initialize(*window);

	window->setView(sf::View(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2), (sf::Vector2f)window->getSize()));

	AssetManager::getInstance().loadTexture("BackgroundImage", "Assets\\Textures\\TitleScreenBackground.png");
	
	if (!AssetManager::getInstance().Textures["BackgroundImage"])
	{
		std::cout << "Background-Image not found " << std::endl;
	}

		sf::Texture bgTexture = *AssetManager::getInstance().Textures["BackgroundImage"];
		backgroundSprite.setTexture(bgTexture);


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
}
