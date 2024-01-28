#pragma once
#include "GameObject.h"

struct Manifold
{
	std::shared_ptr<GameObject> body1{};
	std::shared_ptr<GameObject> body2{};

	float penetration{};
	sf::Vector2f normal;
};