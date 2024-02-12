//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "StatusEffectCP.h"
#include "../Transformation_Components/TransformationCP.h"

void StatusEffectCP::init()
{
	m_sprite.setScale(1, 1);
}

void StatusEffectCP::update(float deltaTime)
{
	if (!m_gameObject.expired())
	{
		std::shared_ptr<GameObject> go = m_gameObject.lock();
		std::shared_ptr<TransformationCP> trans = std::dynamic_pointer_cast<TransformationCP>(go->getComponentsOfType<TransformationCP>().at(0));
		m_sprite.setPosition(trans->getPosition().x - m_offsetX, trans->getPosition().y - m_offsetY);
	}
}