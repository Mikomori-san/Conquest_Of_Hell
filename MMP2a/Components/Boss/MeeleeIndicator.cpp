#include "stdafx.h"
#include "MeeleeIndicator.h"
#include "../../Manager/AssetManager.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include <iostream>
void MeeleeIndicator::init()
{
	std::string texName = "indicatorMeelee";
	if (!AssetManager::getInstance().Textures[texName])
	{
		AssetManager::getInstance().loadTexture(texName, "Assets\\Textures\\meeleeIndicator.png");
	}

	texture = *AssetManager::getInstance().Textures[texName];
	sprite.setTexture(texture);

	sprite.setScale(2, 2);
	sprite.setTextureRect(sf::IntRect(0, 0, sprite.getTextureRect().width / 4, sprite.getTextureRect().height));
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	sprite.setPosition(sf::Vector2f(469.f  , 100.f ));
	

}

void MeeleeIndicator::update(float deltaTime)
{
	ttl -= deltaTime;
	animationTimeIndex += deltaTime * animationSpeed;

	auto animationFrame = (int)animationTimeIndex % 4; //4->animationFrames for heartSpriteSheetIndicator.png
	std::cout << animationFrame << std::endl;

	sprite.setTextureRect(sf::IntRect(
		animationFrame * sprite.getTextureRect().width,
		0,
		sprite.getTextureRect().width,
		sprite.getTextureRect().height
	));


}

void MeeleeIndicator::setAlive()
{
	alive = true;
	ttl = 3;
	animationTimeIndex = 0;
}

void MeeleeIndicator::setDead()
{
	alive = false;
}
