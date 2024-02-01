#include "stdafx.h"
#include "CharmBA.h"
#include "../../Manager/AssetManager.h"
#include "../Input_Components/InputCP.h"
#include "../Player_Components/DashCP.h"
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
	if (charmed)
	{
		charmDuration -= deltaTime;
		if (charmDuration <= 0)
		{
			charmDuration = 1;
			uncharmPlayer();
		}
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
	if (!playerPtr.expired())
	{
		std::shared_ptr<GameObject> go = playerPtr.lock();
		std::shared_ptr<InputCP> input = std::dynamic_pointer_cast<InputCP>(go->getComponentsOfType<InputCP>().at(0));
		std::shared_ptr<DashCP<sf::Keyboard::Key>> dash = std::dynamic_pointer_cast<DashCP<sf::Keyboard::Key>>(go->getComponentsOfType<DashCP<sf::Keyboard::Key>>().at(0));
		
		if (input && dash)
		{
			if (!dash->getHasIFrames())
			{
				std::cout << "Charm" << std::endl;
				input->toggleInputLock();
				charmed = true;
			}
		}
	}
}

void CharmBA::uncharmPlayer()
{
	std::cout << "Uncharm" << std::endl;
	if (!playerPtr.expired())
	{
		std::shared_ptr<GameObject> go = playerPtr.lock();
		std::shared_ptr<InputCP> input = std::dynamic_pointer_cast<InputCP>(go->getComponentsOfType<InputCP>().at(0));
		if (input)
		{
			input->toggleInputLock();
			charmed = false;
		}
	}
}