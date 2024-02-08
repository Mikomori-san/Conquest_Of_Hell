#include "stdafx.h"
#include "CharmIndicator.h"
#include "../../Manager/AssetManager.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include <iostream>
void CharmIndicator::init()
{
	std::string texName = "indicator";
	if (!AssetManager::getInstance().Textures["indicator"])
	{
		AssetManager::getInstance().loadTexture("indicator", "Assets\\Textures\\heartSpriteSheetIndicator.png");
	}

	texture = *AssetManager::getInstance().Textures["indicator"];
	sprite.setTexture(texture);

	//sprite.setScale(4, 4);
	sprite.setTextureRect(sf::IntRect(0, 0, sprite.getTextureRect().width / 4, sprite.getTextureRect().height));
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	sprite.setPosition(sf::Vector2f(469.f + 10.f , 100.f + 25.f));
	sprite.setScale(6.f, 6.f);

}

void CharmIndicator::update(float deltaTime)
{
	ttl -= deltaTime;
	animationTimeIndex += deltaTime * animationSpeed;

	auto animationFrame = (int)animationTimeIndex % 4; //4->animationFrames for fireCharmIndicator.png
	std::cout << animationFrame << std::endl;

	sprite.setTextureRect(sf::IntRect(
		animationFrame * sprite.getTextureRect().width,
		0,
		sprite.getTextureRect().width,
		sprite.getTextureRect().height
	));
	
	
}

void CharmIndicator::setAlive()
{
	alive = true;
	ttl = 3;
	animationTimeIndex = 0;
}

void CharmIndicator::setDead()
{
	alive = false;
}
