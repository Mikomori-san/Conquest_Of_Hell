#include "stdafx.h"
#include "LayerCP.h"

void LayerCP::draw()
{
	for (const auto& sprite : layer)
	{
		window->draw(*sprite);
	}
}

void LayerCP::update(float deltaTime)
{

}

void LayerCP::init()
{
}
