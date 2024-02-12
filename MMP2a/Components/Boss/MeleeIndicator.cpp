//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include "../../Manager/AssetManager.h"
#include "BossAttackCP.h"
#include "MeleeIndicator.h"

void MeleeIndicator::init()
{
	std::string texName = "indicatorMeelee";
	if (!AssetManager::getInstance().Textures[texName])
	{
		AssetManager::getInstance().loadTexture(texName, "Assets\\Textures\\meeleeIndicator.png");
	}

	sprite = sf::Sprite(*AssetManager::getInstance().Textures.at(texName));

	sprite.setScale(2, 2);
	sprite.setTextureRect(sf::IntRect(0, 0, sprite.getTextureRect().width / animationFrames, sprite.getTextureRect().height));
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	sf::Vector2f spriteOffset(-20.f, 0.f);
	if (!bossPtr.expired())
	{
		std::shared_ptr<GameObject> boss = bossPtr.lock();
		std::shared_ptr<TransformationCP> transBoss = boss->getComponentsOfType<TransformationCP>().at(0);
		
		sprite.setPosition(transBoss->getPosition() + spriteOffset);
		
	}
}

void MeleeIndicator::update(float deltaTime)
{
	sprite.setTextureRect(sf::IntRect(
		animationFrame * sprite.getTextureRect().width,
		0,
		sprite.getTextureRect().width,
		sprite.getTextureRect().height
	));
}

void MeleeIndicator::setAlive()
{
	alive = true;
}

void MeleeIndicator::setDead()
{
	alive = false;
	animationFrame = 0;
}
