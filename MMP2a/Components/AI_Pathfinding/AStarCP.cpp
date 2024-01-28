#include "stdafx.h"
#include "AStarCP.h"

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "../Transformation_Components/TransformationCP.h"
#include "SteeringCP.h"

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

std::vector<Point> aStar(const std::vector<std::vector<int>>& grid, const Point& start, const Point& goal) {

    const int costs[3] = { 1, 1, 1 };

    if (start.x < 0 || start.x >= grid[0].size() || start.y < 0 || start.y >= grid.size() ||
        goal.x < 0 || goal.x >= grid[0].size() || goal.y < 0 || goal.y >= grid.size())
    {
        std::cerr << "Ungültige Start- oder Zielposition!" << std::endl;
        return {};
    }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;

    std::vector<std::vector<Point>> cameFrom(grid.size(), std::vector<Point>(grid[0].size(), { -1, -1 }));

    std::vector<std::vector<int>> gValues(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));

    if (start.x == goal.x && start.y == goal.y)
    {
        std::cout << "Goal Reached" << std::endl;
        return{};
    }
    Node startNode(start, 0, eucledianDistance(start, goal));
    openList.push(startNode);
    gValues[start.x][start.y] = 0;

    while (!openList.empty()) {

        Node current = openList.top();
        openList.pop();

        if (current.point.x == goal.x && current.point.y == goal.y) {

            std::vector<Point> path;
            Point currentPoint = goal;

            while (!(currentPoint.x == -1 && currentPoint.y == -1)) {
                path.push_back(currentPoint);
                currentPoint = cameFrom[currentPoint.x][currentPoint.y];
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {

                if ((i == 0 && j == 0) || (i != 0 && j != 0)) {
                    continue;
                }

                int newX = current.point.x + i;
                int newY = current.point.y + j;

                if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[0].size() && grid[newX][newY] == 0) {
                    int newG = current.g + costs[(i != 0) ? 0 : 1]; 

                    if (newG < gValues[newX][newY]) {
                        gValues[newX][newY] = newG;

                        Node neighbor({ newX, newY }, newG, eucledianDistance({ newX, newY }, goal));
                        openList.push(neighbor);
                        cameFrom[newX][newY] = current.point;
                    }
                }
            }
        }
    }

    std::cerr << "Kein Pfad gefunden!" << std::endl;
    return {};
}


void AStarCP::update(float deltaTime)
{
    if (!gameObject.expired())
    {
        auto go = gameObject.lock();

        sf::Vector2f myPos = go->getComponentsOfType<TransformationCP>().at(0)->getPosition();

        sf::Vector2i myPosGrid = sf::Vector2i(myPos.x / tileSize, myPos.y / tileSize);
        sf::Vector2i targetPosGrid = sf::Vector2i(targetPos.x / tileSize, targetPos.y / tileSize);

        Point myPoint = Point(myPosGrid.x, myPosGrid.y);
        Point targetPoint = Point(targetPosGrid.x, targetPosGrid.y);

        Point nextPoint;
            
        std::vector<Point> targetPoints = aStar(grid, myPoint, targetPoint);

        if (targetPoints.size() > 1)
        {
            nextPoint = targetPoints[1];
            go->getComponentsOfType<SteeringCP>().at(0)->setDestination(sf::Vector2f(nextPoint.x * tileSize + tileSize / 2, nextPoint.y * tileSize + tileSize / 2));
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
    timer = 0;

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            for (auto& pos : unmovablePositions)
            {
                if (i == pos.x && j == pos.y)
                {
                    grid[i][j] = std::numeric_limits<float>().max();
                    break;
                }
            }
        }
    }
}
