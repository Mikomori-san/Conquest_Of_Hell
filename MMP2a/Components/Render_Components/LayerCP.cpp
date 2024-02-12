//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "LayerCP.h"

void LayerCP::draw()
{
	auto centerPoint = window->getView().getCenter();
	auto viewSize = window->getView().getSize();

	for (const auto& sprite : layer)
	{
		auto pos = sprite->getPosition();

		if(pos.x <= centerPoint.x + viewSize.x / 2 + 16 &&
			pos.x >= centerPoint.x - viewSize.x / 2 - 16 &&
			pos.y <= centerPoint.y + viewSize.y / 2 + 16 &&
			pos.y >= centerPoint.y - viewSize.y / 2 - 16
		)
			window->draw(*sprite);
	}
}

void LayerCP::update(float deltaTime)
{

}

void LayerCP::init()
{
}
