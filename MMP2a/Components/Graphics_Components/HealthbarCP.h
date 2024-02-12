//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "GraphicsCP.h"

class HealthbarCP : public GraphicsCP
{
public:
	HealthbarCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture) : GraphicsCP(gameObject,id,texture) {}
	void init() override;
	void update(float deltaTime) override;
private:
	void scaleHealthbar();
	float origWidth = 1;
	int offset = 20; //used to offset the pos of healthbar above the player
};
