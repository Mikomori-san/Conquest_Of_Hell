//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../StatsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "HealthbarCP.h"

void HealthbarCP::init()
{
	m_sprite->setScale(1,1);
	m_origWidth = m_sprite->getTextureRect().width;
}

void HealthbarCP::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		std::shared_ptr<TransformationCP> trans = std::dynamic_pointer_cast<TransformationCP>(go->getComponentsOfType<TransformationCP>().at(0));
		m_sprite->setPosition(trans->getPosition().x, trans->getPosition().y - m_offset);
	}
	scaleHealthbar();
}

void HealthbarCP::scaleHealthbar()
{
	std::shared_ptr<StatsCP> stats = std::dynamic_pointer_cast<StatsCP>(gameObject.lock()->getComponentsOfType<StatsCP>().at(0));
	float ratio = (float)stats->getHealth() / (float)stats->getMaxHealth();
	m_sprite->setTextureRect(sf::IntRect(m_sprite->getTextureRect().left, m_sprite->getTextureRect().top, m_origWidth*ratio , m_sprite->getTextureRect().height));
}
