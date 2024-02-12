//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"

using Grid = std::vector<std::vector<int>>;

class AStarCP : public Component 
{
public:
	AStarCP(std::weak_ptr<GameObject> incGameObject, std::string incId, Grid incGrid, std::vector<sf::Vector2i>& unMovablePositions, sf::Vector2f incTargPos, int incTileSize) : Component(incGameObject, incId),
		m_grid(incGrid), m_unmovablePositions(unMovablePositions), m_targetPos(incTargPos), m_tileSize(incTileSize), m_timer(0)
	{};

	void update(float deltaTime) override;
	std::string getComponentId() override;
	void setComponentId(std::string id) override;
	void init() override;

	void setTargetPos(sf::Vector2f targetPos) { this->m_targetPos = targetPos; }

	void updatePositions(std::vector<sf::Vector2i> incSkelPos);
private:
	Grid m_grid;
	sf::Vector2f m_targetPos;
	std::vector<sf::Vector2i> m_unmovablePositions;
	std::vector<sf::Vector2i> m_otherSkeletonPositions;
	int m_tileSize;
	float m_timer;
};