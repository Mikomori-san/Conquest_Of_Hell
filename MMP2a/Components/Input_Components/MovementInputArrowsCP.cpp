#include "stdafx.h"
#include "MovementInputArrowsCP.h"
#include <iostream>
#include "../DecisionHandlerCP.h"

void MovementInputArrowsCP::update(float deltatime)
{
    if (!gameObject.expired())
    {
        vel = gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->getOriginalVelocity();

        if (!inputLock)
        {
            processInput();
        }
        else
        {
            if (hadInput)
            {
                gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleMovement(lastDirection, lastVec, vel);
            }
            else
            {
                gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleIdle(lastDirection);
            }
        }
    }
}

void MovementInputArrowsCP::processInput()
{
    if (!gameObject.expired())
    {
        gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->setVelocity(0);

        std::shared_ptr<DecisionHandlerCP> decHandler = gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0);

        if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Up) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Right))
        {
            decHandler->handleMovement(Direction::upRight, sf::Vector2f(0.75f, -0.75f), vel);
            lastDirection = Direction::upRight;
            lastVec = sf::Vector2f(0.75f, -0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Right) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Down))
        {
            decHandler->handleMovement(Direction::rightDown, sf::Vector2f(0.75f, 0.75f), vel);
            lastDirection = Direction::rightDown;
            lastVec = sf::Vector2f(0.75f, 0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Down) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Left))
        {
            decHandler->handleMovement(Direction::downLeft, sf::Vector2f(-0.75f, 0.75f), vel);
            lastDirection = Direction::downLeft;
            lastVec = sf::Vector2f(-0.75f, 0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Left) && InputManager::getInstance().getKeyPressed(sf::Keyboard::Up))
        {
            decHandler->handleMovement(Direction::leftUp, sf::Vector2f(-0.75f, -0.75f), vel);
            lastDirection = Direction::leftUp;
            lastVec = sf::Vector2f(-0.75f, -0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Up))
        {
            decHandler->handleMovement(lastDirection, sf::Vector2f(0, -1), vel);
            lastVec = sf::Vector2f(0, -1);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Left))
        {
            decHandler->handleMovement(Direction::left, sf::Vector2f(-1, 0), vel);
            lastDirection = Direction::left;
            lastVec = sf::Vector2f(-1, 0);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Down))
        {
            decHandler->handleMovement(lastDirection, sf::Vector2f(0, -1), vel);
            lastVec = sf::Vector2f(0, 1);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::Right))
        {
            decHandler->handleMovement(Direction::right, sf::Vector2f(1, 0), vel);
            lastDirection = Direction::right;
            lastVec = sf::Vector2f(1, 0);
        }
        else
        {
            hadInput = false;
            gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleIdle(lastDirection);
        }
    }
}

void MovementInputArrowsCP::init()
{

}