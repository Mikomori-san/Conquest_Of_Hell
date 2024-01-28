#pragma once

class DebugDraw
{
public:
    static DebugDraw& getInstance()
    {
        static DebugDraw instance;
        return instance;
    }

    void initialize(sf::RenderWindow& renderWindow);
    void drawLine(sf::Vector2f startPoint, sf::Vector2f endPoint, sf::Color color);
    void drawRectOutline(sf::Vector2f position, int width, int height, sf::Color color);
    void drawRectOutline(sf::FloatRect intRect, sf::Color color);
    void drawRectangle(sf::Vector2f position, int width, int height, sf::Color color);
    void drawRectangle(sf::IntRect rect, sf::Color color);
    void unload();

private:
    DebugDraw() = default;
    ~DebugDraw() = default;
    DebugDraw(const DebugDraw&) = delete;
    DebugDraw& operator=(const DebugDraw&) = delete;

    sf::RenderWindow* window;
};