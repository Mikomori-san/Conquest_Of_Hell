//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "GraphicsCP.h"

void GraphicsCP::update(float deltaTime)
{

}

void GraphicsCP::init()
{

}

void GraphicsCP::setSprite(std::shared_ptr<sf::Texture> texture)
{
	this->sprite->setTexture(*texture);
}
