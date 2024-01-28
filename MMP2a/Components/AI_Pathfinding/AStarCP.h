#pragma once
#include "../Component.h"

using Grid = std::vector<std::vector<int>>;

class AStarCP : public Component 
{
public:
	AStarCP(std::weak_ptr<GameObject> incGameObject, std::string incId, Grid incGrid, std::vector<sf::Vector2i>& unMovablePositions, sf::Vector2f incTargPos, int incTileSize) : Component(incGameObject, incId),
		grid(incGrid), unmovablePositions(unMovablePositions), targetPos(incTargPos), tileSize(incTileSize), timer(0)
	{};

	void update(float deltaTime) override;
	std::string getComponentId() override;
	void setComponentId(std::string id) override;
	void init() override;

	void setTargetPos(sf::Vector2f targetPos) { this->targetPos = targetPos; }
	
	void reset() { timer = 0; }
private:
	Grid grid;
	sf::Vector2f targetPos;
	std::vector<sf::Vector2i> unmovablePositions;
	int tileSize;
	float timer;
};