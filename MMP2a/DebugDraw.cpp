#include "stdafx.h"
#include "DebugDraw.h"

void DebugDraw::initialize(sf::RenderWindow& renderWindow)
{
	window = &renderWindow;
}

void DebugDraw::drawLine(sf::Vector2f startPoint, sf::Vector2f endPoint, sf::Color color)
{
	sf::Vertex line[] = {
		sf::Vertex(startPoint, color),
		sf::Vertex(endPoint, color)
	};
	window->draw(line, 2, sf::Lines);
}

void DebugDraw::drawRectOutline(sf::Vector2f position, int width, int height, sf::Color color)
{
	auto bottomLeftPos = sf::Vector2f(position.x, position.y + height);
	auto topLeftPos = sf::Vector2f(position.x, position.y);
	auto topRightPos = sf::Vector2f(position.x + width, position.y);
	auto bottomRightPos = sf::Vector2f(position.x + width, position.y + height);

	sf::Vertex line[] = {
		sf::Vertex(bottomLeftPos, color),
		sf::Vertex(topLeftPos, color)
	};
	window->draw(line, 2, sf::Lines);

	line[0] = sf::Vertex(topLeftPos, color);
	line[1] = sf::Vertex(topRightPos, color);
	window->draw(line, 2, sf::Lines);

	line[0] = sf::Vertex(topRightPos, color);
	line[1] = sf::Vertex(bottomRightPos, color);
	window->draw(line, 2, sf::Lines);

	line[0] = sf::Vertex(bottomRightPos, color);
	line[1] = sf::Vertex(bottomLeftPos, color);
	window->draw(line, 2, sf::Lines);
}

void DebugDraw::drawRectOutline(sf::FloatRect intRect, sf::Color color)
{
	auto position = sf::Vector2f(intRect.left, intRect.top);
	auto width = intRect.width;
	auto height = intRect.height;

	auto bottomLeftPos = sf::Vector2f(position.x, position.y + height);
	auto topLeftPos = sf::Vector2f(position.x, position.y);
	auto topRightPos = sf::Vector2f(position.x + width, position.y);
	auto bottomRightPos = sf::Vector2f(position.x + width, position.y + height);

	sf::Vertex line[] = {
		sf::Vertex(bottomLeftPos, color),
		sf::Vertex(topLeftPos, color)
	};
	window->draw(line, 2, sf::Lines);

	line[0] = sf::Vertex(topLeftPos, color);
	line[1] = sf::Vertex(topRightPos, color);
	window->draw(line, 2, sf::Lines);

	line[0] = sf::Vertex(topRightPos, color);
	line[1] = sf::Vertex(bottomRightPos, color);
	window->draw(line, 2, sf::Lines);

	line[0] = sf::Vertex(bottomRightPos, color);
	line[1] = sf::Vertex(bottomLeftPos, color);
	window->draw(line, 2, sf::Lines);
}

void DebugDraw::drawRectangle(sf::Vector2f position, int width, int height, sf::Color color)
{
	auto rectangle = sf::RectangleShape(sf::Vector2f(width, height));
	rectangle.setPosition(position);
	rectangle.setFillColor(color);
	window->draw(rectangle);
}

void DebugDraw::drawRectangle(sf::IntRect rect, sf::Color color)
{
	auto rectangle = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
	rectangle.setPosition(sf::Vector2f(rect.left, rect.top));
	rectangle.setFillColor(color);
	window->draw(rectangle);
}

void DebugDraw::unload()
{
	this->window = nullptr;
}