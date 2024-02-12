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
	posResetTimer++;
	if (posResetTimer > 5)
	{
		oldPos = position;
		posResetTimer = 0;
	}

	// for AI
	if (backupVel != 0 && !velLock)
	{
		curVelocity = backupVel;
	}
	if (velLock)
		std::cout << "Vel Lock active!" << std::endl;

	position = position + direction * curVelocity * deltaTime;
	
	if (rigid)
	{
		rigid->setPos(position);
	}
}

void TransformationCP::init()
{
	velLock = false;
	position = originalPos;
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		if (go->getComponentsOfType<GraphicsCP>().size() != 0)
		{
			std::shared_ptr<GraphicsCP> ani = go->getComponentsOfType<GraphicsCP>().at(0);
			origin = sf::Vector2f(ani->getSprite().getTextureRect().width / 2, ani->getSprite().getTextureRect().height / 2);
			setOrigin(origin);
		}
		
		if (go->getComponentsOfType<RigidBodyCP>().size() != 0)
		{
			rigid = go->getComponentsOfType<RigidBodyCP>().at(0);
		}
	}
}