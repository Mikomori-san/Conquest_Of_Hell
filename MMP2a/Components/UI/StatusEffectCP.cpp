//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "StatusEffectCP.h"
#include "../Transformation_Components/TransformationCP.h"

void StatusEffectCP::init()
{
	sprite.setScale(1, 1);
}

void StatusEffectCP::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		std::shared_ptr<TransformationCP> trans = std::dynamic_pointer_cast<TransformationCP>(go->getComponentsOfType<TransformationCP>().at(0));
		sprite.setPosition(trans->getPosition().x - offsetX, trans->getPosition().y - offsetY);
	}
}