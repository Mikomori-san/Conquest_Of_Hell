//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../StatsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "HealthbarCP.h"

void HealthbarCP::init()
{
	sprite->setScale(1,1);
	origWidth = sprite->getTextureRect().width;
}

void HealthbarCP::update(float deltaTime)
{
	if (!gameObject.expired())
	{
		std::shared_ptr<GameObject> go = gameObject.lock();
		std::shared_ptr<TransformationCP> trans = std::dynamic_pointer_cast<TransformationCP>(go->getComponentsOfType<TransformationCP>().at(0));
		sprite->setPosition(trans->getPosition().x, trans->getPosition().y - offset);
	}
	scaleHealthbar();
}

void HealthbarCP::scaleHealthbar()
{
	std::shared_ptr<StatsCP> stats = std::dynamic_pointer_cast<StatsCP>(gameObject.lock()->getComponentsOfType<StatsCP>().at(0));
	float ratio = (float)stats->getHealth() / (float)stats->getMaxHealth();
	sprite->setTextureRect(sf::IntRect(sprite->getTextureRect().left, sprite->getTextureRect().top, origWidth*ratio , sprite->getTextureRect().height));
}
