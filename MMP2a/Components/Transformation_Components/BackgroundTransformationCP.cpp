#include "stdafx.h"
#include "BackgroundTransformationCP.h"
#include "../Graphics_Components/StandardGraphicsCP.h"


void BackgroundTransformationCP::init()
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		std::shared_ptr<StandardGraphicsCP> BackgroundGCP = std::dynamic_pointer_cast<StandardGraphicsCP>(go->getComponentsOfType<StandardGraphicsCP>().at(0));
		sf::Vector2f origin(0, 0);
		setOrigin(origin);
	}
}

void BackgroundTransformationCP::update(float deltaTime)
{
	position = position + direction * curVelocity * deltaTime;
}
