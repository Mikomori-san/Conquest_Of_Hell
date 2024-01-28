#pragma once
#include <vector>
#include <memory>
#include "../Manifold.h"

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