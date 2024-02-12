//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Collision_Components/RigidBodyCP.h"
#include "../Graphics_Components/GraphicsCP.h"
#include "TransformationCP.h"

void TransformationCP::update(float deltaTime)
{
	// for positional correction when hitting a boundary
	m_posResetTimer++;
	if (m_posResetTimer > 5)
	{
		m_oldPos = m_position;
		m_posResetTimer = 0;
	}

	// for AI
	if (m_backupVel != 0 && !m_velLock)
	{
		m_curVelocity = m_backupVel;
	}

	m_position = m_position + m_direction * m_curVelocity * deltaTime;
	
	if (m_rigid)
	{
		m_rigid->setPos(m_position);
	}
}

void TransformationCP::init()
{
	m_velLock = false;
	m_position = m_originalPos;
	if (!m_gameObject.expired())
	{
		std::shared_ptr<GameObject> go = m_gameObject.lock();
		if (go->getComponentsOfType<GraphicsCP>().size() != 0)
		{
			std::shared_ptr<GraphicsCP> ani = go->getComponentsOfType<GraphicsCP>().at(0);
			m_origin = sf::Vector2f(ani->getSprite().getTextureRect().width / 2, ani->getSprite().getTextureRect().height / 2);
			setOrigin(m_origin);
		}
		
		if (go->getComponentsOfType<RigidBodyCP>().size() != 0)
		{
			m_rigid = go->getComponentsOfType<RigidBodyCP>().at(0);
		}
	}
}