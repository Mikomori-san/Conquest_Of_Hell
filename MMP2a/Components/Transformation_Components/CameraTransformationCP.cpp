//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "CameraTransformationCP.h"
#include "../CameraCP.h"

void CameraTransformationCP::init()
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		setOrigin(sf::Vector2f(go->getComponentsOfType<CameraCP>().at(0)->getWindow()->getPosition()));
	}
}

void CameraTransformationCP::update(float deltaTime)
{
	position = position + direction * curVelocity * deltaTime;
}
