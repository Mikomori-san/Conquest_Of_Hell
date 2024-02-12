//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include "../../Manager/AssetManager.h"
#include "../Input_Components/InputCP.h"
#include "../UI/StatusEffectCP.h"
#include "CharmBA.h"
#include <iostream>


int findRotation(sf::Vector2f direction)
{
	float angle = std::atan2(direction.y, direction.x);
	float PI = 3.141;
	int rotation = static_cast<int>(angle * (90.f / static_cast<float>(PI)));
	return rotation;
}

void CharmBA::init()
{
	m_abilityType = Charm;

	std::string texName = "heartSprite";
	if (!AssetManager::getInstance().Textures[texName])
	{
		AssetManager::getInstance().loadTexture(texName, "Assets\\Textures\\heartSpriteSheet.png");
	}

	m_sprite = sf::Sprite(*AssetManager::getInstance().Textures[texName]);
	m_sprite.setScale(4,4);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite.getTextureRect().width / 4, m_sprite.getTextureRect().height));
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
	
	AssetManager::getInstance().Music["Charm"]->setVolume(8);
}

void CharmBA::update(float deltaTime)
{
	m_ttl -= deltaTime;
	animationTimeIndex += deltaTime * animationSpeed;
	m_position += m_direction * MOVE_SPEED * deltaTime;

	if (m_ttl <= 0)
	{
		setDead();
	}
	else
	{

		auto animationFrame = (int)animationTimeIndex % 4; //4->animationFrames for heartSpriteSheet.png
		m_sprite.setTextureRect(sf::IntRect(
			animationFrame * m_sprite.getTextureRect().width,
			0,
			m_sprite.getTextureRect().width,
			m_sprite.getTextureRect().height
		));
		m_sprite.setPosition(m_position);
		m_sprite.setRotation(findRotation(m_direction));

	}
	if (charmed)
	{
		if (!m_charmHit)
		{
			AssetManager::getInstance().Music["Charm"]->play();
			m_charmHit = true;
		}
		m_charmDuration -= deltaTime;
		if (m_charmDuration <= 0)
		{
			m_charmDuration = 1;
			uncharmPlayer();
			m_charmHit = false;
		}
	}

}

void CharmBA::execute()
{
	charmPlayer();
}

const sf::FloatRect& CharmBA::getHitbox()
{
	m_hitbox = m_sprite.getGlobalBounds();
	m_hitbox.width /= 3;
	m_hitbox.height /= 3;
	m_hitbox.left += m_hitbox.width;
	m_hitbox.top += m_hitbox.height;
	return m_hitbox;
}

void CharmBA::setAlive()
{
	alive = true;
	m_ttl = 3;
	animationTimeIndex = 0;
}

void CharmBA::setDead()
{
	alive = false;
	m_position = sf::Vector2f(0, 0);
}

void CharmBA::charmPlayer()
{
	if (!m_playerPtr.expired())
	{
		std::shared_ptr<GameObject> go = m_playerPtr.lock();
		std::shared_ptr<InputCP> input = std::dynamic_pointer_cast<InputCP>(go->getComponentsOfType<InputCP>().at(0));
		if (input)
		{
				input->toggleInputLock();
				charmed = true;
				go->getComponentsOfType<StatusEffectCP>().at(0)->toggleDisplayStatus();
		}
		
	}
}

void CharmBA::uncharmPlayer()
{
	if (!m_playerPtr.expired())
	{
		std::shared_ptr<GameObject> go = m_playerPtr.lock();
		std::shared_ptr<InputCP> input = std::dynamic_pointer_cast<InputCP>(go->getComponentsOfType<InputCP>().at(0));
		if (input)
		{
			input->toggleInputLock();
			charmed = false;
			go->getComponentsOfType<StatusEffectCP>().at(0)->toggleDisplayStatus();
		}
	}
}