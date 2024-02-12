//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../../Components/Transformation_Components/TransformationCP.h"
#include "../../Manager/AssetManager.h"
#include "CharmIndicator.h"

void CharmIndicator::init()
{
	if(!m_bossPtr.expired())
	{ 
		std::shared_ptr<GameObject> boss = m_bossPtr.lock();
		std::shared_ptr<TransformationCP> transBoss = boss->getComponentsOfType<TransformationCP>().at(0);

		std::string texName = "indicatorCharm";
		if (!AssetManager::getInstance().Textures[texName])
		{
			AssetManager::getInstance().loadTexture(texName, "Assets\\Textures\\heartSpriteSheetIndicator.png");
		}
		m_sprite.setTexture(*AssetManager::getInstance().Textures[texName]);

		m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite.getTextureRect().width / ANIMATION_FRAMES, m_sprite.getTextureRect().height));
		m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
		m_sprite.setPosition(sf::Vector2f(transBoss->getPosition() + OFFSET));
		m_sprite.setScale(6.f, 6.f);
	}
}

void CharmIndicator::update(float deltaTime)
{
	m_animationTimeIndex += deltaTime * m_animationSpeed;

	auto animationFrame = (int)m_animationTimeIndex % ANIMATION_FRAMES;

	m_sprite.setTextureRect(sf::IntRect(
		animationFrame * m_sprite.getTextureRect().width,
		0,
		m_sprite.getTextureRect().width,
		m_sprite.getTextureRect().height
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
