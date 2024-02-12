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

	m_sprite = sf::Sprite(*AssetManager::getInstance().Textures.at(texName));

	m_sprite.setScale(2, 2);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite.getTextureRect().width / ANIMATION_FRAMES, m_sprite.getTextureRect().height));
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
	sf::Vector2f spriteOffset(-20.f, 0.f);
	if (!m_bossPtr.expired())
	{
		std::shared_ptr<GameObject> boss = m_bossPtr.lock();
		std::shared_ptr<TransformationCP> transBoss = boss->getComponentsOfType<TransformationCP>().at(0);
		
		m_sprite.setPosition(transBoss->getPosition() + spriteOffset);
		
	}
}

void MeleeIndicator::update(float deltaTime)
{
	m_sprite.setTextureRect(sf::IntRect(
		animationFrame * m_sprite.getTextureRect().width,
		0,
		m_sprite.getTextureRect().width,
		m_sprite.getTextureRect().height
	));
}

void MeleeIndicator::setAlive()
{
	m_alive = true;
}

void MeleeIndicator::setDead()
{
	m_alive = false;
	animationFrame = 0;
}
