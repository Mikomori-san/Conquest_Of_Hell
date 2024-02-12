#include "stdafx.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

namespace Utils
{
    // Returns the squared magnitude of a vector (length before square root)
    float SqrMagnitude(const sf::Vector2f& input)
    {
        float sqrLength = input.x * input.x + input.y * input.y;
        return sqrLength;
    }

    // Rotates a vector by a given angle counterclockwise
    sf::Vector2f RotateVector(const sf::Vector2f& v, float angle)
    {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return sf::Vector2f(v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA);
    }

    // Returns the angle between two vectors in radians
    float AngleBetween(const sf::Vector2f& v1, const sf::Vector2f& v2)
    {
        return std::atan2(v2.y - v1.y, v2.x - v1.x);
    }

    // Converts radians to degrees
    float ToDegrees(float radians)
    {
        return 180.f / 3.14159265f * radians;
    }

    // Converts degrees to radians
    float ToRadians(float degrees)
    {
        return 3.14159265f / 180.f * degrees;
    }

    // Distance between two points
    float Distance(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // Normalize a vector to a length of 1
    sf::Vector2f Normalize(const sf::Vector2f& source)
    {
        float length = std::sqrt(source.x * source.x + source.y * source.y);
        if (length != 0.f)
            return sf::Vector2f(source.x / length, source.y / length);
        return source;
    }

    // Linear interpolation between two floats
    float Lerp(float firstFloat, float secondFloat, float t, bool clamped = true)
    {
        if (clamped)
            t = t > 1.0f ? 1.0f : t;
        return firstFloat * (1 - t) + secondFloat * t;
    }

    // Linear interpolation between two vectors
    sf::Vector2f Lerp(const sf::Vector2f& firstVector, const sf::Vector2f& secondVector, float t)
    {
        float x = Lerp(firstVector.x, secondVector.x, t);
        float y = Lerp(firstVector.y, secondVector.y, t);
        return sf::Vector2f(x, y);
    }

    // Dot Product of two vectors
    float Dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }
}
