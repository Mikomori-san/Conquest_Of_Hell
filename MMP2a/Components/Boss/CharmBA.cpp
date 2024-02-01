#include "stdafx.h"
#include "CharmBA.h"
#include "../../Manager/AssetManager.h"
int findRotation(sf::Vector2f direction)
{
	float angle = std::atan2(direction.y, direction.x);
	float PI = 3.141;
	int rotation = static_cast<int>(angle * (90.f / static_cast<float>(PI)));
	return rotation;
}

void CharmBA::init()
{
	abilityType = Charm;

	std::string texName = "heartSprite";
	if (!AssetManager::getInstance().Textures[texName])
	{
		AssetManager::getInstance().loadTexture(texName, "Assets\\Textures\\heartSpriteSheet.png");
	}
	sprite = sf::Sprite(*AssetManager::getInstance().Textures[texName]);
	sprite.setScale(4,4);
	sprite.setTextureRect(sf::IntRect(0, 0, sprite.getTextureRect().width / 4, sprite.getTextureRect().height));
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
}

void CharmBA::update(float deltaTime)
{
	ttl -= deltaTime;
	animationTimeIndex += deltaTime * animationSpeed;
	position += direction * moveSpeed * deltaTime;

	if (ttl <= 0)
	{
		setDead();
	}
	else
	{
		auto animationFrame = (int)animationTimeIndex % 4; //4->animationFrames for heartSpriteSheet.png
		sprite.setTextureRect(sf::IntRect(
			animationFrame * sprite.getTextureRect().width,
			0,
			sprite.getTextureRect().width,
			sprite.getTextureRect().height
		));
		sprite.setPosition(position);
		sprite.setRotation(findRotation(direction));
	}

}

void CharmBA::execute()
{
	charmPlayer();
}

const sf::FloatRect& CharmBA::getHitbox()
{
	hitbox = sprite.getGlobalBounds();
	hitbox.width /= 3;
	hitbox.height /= 3;
	hitbox.left += hitbox.width;
	hitbox.top += hitbox.height;
	return hitbox;
}

void CharmBA::setAlive()
{
	alive = true;
	ttl = 3;
	animationTimeIndex = 0;
}

void CharmBA::setDead()
{
	alive = false;
	position = sf::Vector2f(0, 0);
}

void CharmBA::charmPlayer()
{
	std::cout << "Charm" << std::endl;
	//->playerLogic
}
