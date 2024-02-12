//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Manifold.h"
#include <memory>
#include <vector>

class PhysicsManager
{
public:
	static PhysicsManager& getInstance()
	{
		static PhysicsManager instance;
		return instance;
	}

	void update(std::vector<std::shared_ptr<GameObject>> gameObjects, float deltaTime);

private:
	PhysicsManager() = default;
	~PhysicsManager() = default;
	std::vector<std::shared_ptr<Manifold>> manifolds;

	void collisionCheck(std::vector<std::shared_ptr<GameObject>>& gameObjects); // store manifolds for collisions in vector
	void collisionResolve(); // resolve manifolds foreach man : manifolds

	float accumulator = 0;
};