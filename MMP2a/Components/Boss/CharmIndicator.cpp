//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include "../../Manager/AssetManager.h"
#include "CharmIndicator.h"

void CharmIndicator::init()
{
	if(!bossPtr.expired())
	{ 
		std::shared_ptr<GameObject> boss = bossPtr.lock();
		std::shared_ptr<TransformationCP> transBoss = boss->getComponentsOfType<TransformationCP>().at(0);

		std::string texName = "indicatorCharm";
		if (!AssetManager::getInstance().Textures[texName])
		{
			AssetManager::getInstance().loadTexture(texName, "Assets\\Textures\\heartSpriteSheetIndicator.png");
		}
		sprite.setTexture(*AssetManager::getInstance().Textures[texName]);

		sprite.setTextureRect(sf::IntRect(0, 0, sprite.getTextureRect().width / animationFrames, sprite.getTextureRect().height));
		sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
		sprite.setPosition(sf::Vector2f(transBoss->getPosition() + offset));
		sprite.setScale(6.f, 6.f);
	}
}

void CharmIndicator::update(float deltaTime)
{
	animationTimeIndex += deltaTime * animationSpeed;

	auto animationFrame = (int)animationTimeIndex % animationFrames;

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
}

void CharmIndicator::setDead()
{
	alive = false;
}
