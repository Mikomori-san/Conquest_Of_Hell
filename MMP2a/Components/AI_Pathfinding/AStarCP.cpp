//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "AStarCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "SteeringCP.h"
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>

struct Point {
    int x, y;
    
    Point(int x, int y) : x(x), y(y) {}
    Point() = default;
};

struct Node {
    Point point;
    int g; 
    int h;

    Node(const Point& point, int g, int h) : point(point), g(g), h(h) {}

    int f() const {
        return g + h;
    }

    bool operator>(const Node& other) const {
        return f() > other.f();
    }
};

int eucledianDistance(const Point& a, const Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    //return std::sqrt(std::abs(a.x - b.x) * std::abs(a.x - b.x) + std::abs(a.y - b.y) * std::abs(a.y - b.y)); ==> schlechter, ist aber die eig. euklidische
}

std::vector<Point> aStar(const std::vector<std::vector<int>> grid, const Point start, const Point goal) {
    const int costs[3] = { 1, 1, 1 };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::vector<std::vector<Point>> cameFrom(grid.size(), std::vector<Point>(grid[0].size(), { -1, -1 }));
    std::vector<std::vector<int>> gValues(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));

    openList.push(Node(start, 0, eucledianDistance(start, goal)));
    gValues[start.y][start.x] = 0;

    while (!openList.empty() && openList.size() <= 100) {
        Node current = openList.top();
        openList.pop();

        if (current.point.x == goal.x && current.point.y == goal.y) {
            std::vector<Point> path;
            Point currentPoint = goal;

            while (!(currentPoint.x == -1 && currentPoint.y == -1)) {
                path.push_back(currentPoint);
                currentPoint = cameFrom[currentPoint.y][currentPoint.x];
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) {
                    continue;
                }

                int newX = current.point.x + j;
                int newY = current.point.y + i;

                if (newX >= 0 && newX < grid[0].size() && newY >= 0 && newY < grid.size() && grid[newY][newX] == 0) {
                    int newG = current.g + costs[(i != 0) ? 0 : 1];

                    if (newG < gValues[newY][newX]) {
                        gValues[newY][newX] = newG;

                        Node neighbor({ newX, newY }, newG, eucledianDistance({ newX, newY }, goal));
                        openList.push(neighbor);
                        cameFrom[newY][newX] = current.point;
                    }
                }
            }
        }
    }

    return {}; // Return an empty path if no valid path is found
}

void AStarCP::updatePositions(std::vector<sf::Vector2i> incSkelPos)
{
    if (!gameObject.expired())
    {
        auto myPos = gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->getPosition();
        m_otherSkeletonPositions = {};
        for (auto skelPos : incSkelPos)
        {
            if (skelPos != sf::Vector2i(myPos.x / m_tileSize, myPos.y / m_tileSize))
                m_otherSkeletonPositions.push_back(skelPos);
        }
    }
}

void AStarCP::update(float deltaTime)
{
    std::vector<sf::Vector2i> updatedUnmovables = m_unmovablePositions;
    updatedUnmovables.insert(updatedUnmovables.end(), m_otherSkeletonPositions.begin(), m_otherSkeletonPositions.end());

    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid[0].size(); j++)
        {
            for (auto& pos : updatedUnmovables)
            {
                if (i == pos.x && j == pos.y)
                {
                    m_grid[i][j] = 100;
                    break;
                }
            }
        }
    }

    if (!gameObject.expired())
    {
        auto go = gameObject.lock();

        sf::Vector2f myPos = go->getComponentsOfType<TransformationCP>().at(0)->getPosition();

        sf::Vector2i myPosGrid = sf::Vector2i(myPos.x / m_tileSize, myPos.y / m_tileSize);
        sf::Vector2i targetPosGrid = sf::Vector2i(m_targetPos.x / m_tileSize, m_targetPos.y / m_tileSize);

        Point myPoint = Point(myPosGrid.x, myPosGrid.y);
        Point targetPoint = Point(targetPosGrid.x, targetPosGrid.y);

        Point nextPoint;

        std::vector<Point> targetPoints = aStar(m_grid, myPoint, targetPoint);

        if (targetPoints.size() > 1)
        {
            nextPoint = targetPoints[1];
            go->getComponentsOfType<SteeringCP>().at(0)->setDestination(sf::Vector2f(nextPoint.x * m_tileSize + m_tileSize / 2, nextPoint.y * m_tileSize + m_tileSize / 2));
        } 
        else
        {
            go->getComponentsOfType<SteeringCP>().at(0)->setDestination(sf::Vector2f(myPos.x, myPos.y));
        }
    }
}

std::string AStarCP::getComponentId()
{
    return this->componentId;
}

void AStarCP::setComponentId(std::string id)
{
    this->componentId = id;
}

void AStarCP::init()
{
}
