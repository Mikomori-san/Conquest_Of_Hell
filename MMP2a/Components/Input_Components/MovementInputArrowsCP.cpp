#include "stdafx.h"
#include "MovementInputArrowsCP.h"
#include <iostream>
#include "../DecisionHandlerCP.h"

void MovementInputArrowsCP::update(float deltatime)
{
    processInput();
}

void MovementInputArrowsCP::processInput()
{
    if (!gameObject.expired())
    {
        gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->setVelocity(0);

        float vel = gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->getOriginalVelocity();

        std::shared_ptr<DecisionHandlerCP> decHandler = gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0);

        if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Up) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Right))
        {
            decHandler->handleMovement(Direction::upRight, sf::Vector2f(0.75f, -0.75f), vel);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Right) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Down))
        {
            decHandler->handleMovement(Direction::rightDown, sf::Vector2f(0.75f, 0.75f), vel);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Down) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Left))
        {
            decHandler->handleMovement(Direction::downLeft, sf::Vector2f(-0.75f, 0.75f), vel);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Left) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Up))
        {
            decHandler->handleMovement(Direction::leftUp, sf::Vector2f(-0.75f, -0.75f), vel);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Up))
        {
            decHandler->handleMovement(Direction::up, sf::Vector2f(0, -1), vel);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Left))
        {
            decHandler->handleMovement(Direction::left, sf::Vector2f(-1, 0), vel);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Down))
        {
            decHandler->handleMovement(Direction::down, sf::Vector2f(0, 1), vel);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Right))
        {
            decHandler->handleMovement(Direction::right, sf::Vector2f(1, 0), vel);
        }
    }
}

void MovementInputArrowsCP::init()
{

}