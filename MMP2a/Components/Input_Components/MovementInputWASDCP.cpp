#include "stdafx.h"
#include "MovementInputWASDCP.h"
#include <iostream>
#include "../DecisionHandlerCP.h"

void MovementInputWASDCP::update(float deltatime)
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

void MovementInputWASDCP::processInput()
{
    if (!gameObject.expired())
    {
        gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->setVelocity(0);

        std::shared_ptr<DecisionHandlerCP> decHandler = gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0);

        hadInput = true;

        if (InputManager::getInstance().getKeyPressed(sf::Keyboard::W) && InputManager::getInstance().getKeyPressed(sf::Keyboard::D))
        {
            decHandler->handleMovement(Direction::upRight, sf::Vector2f(0.75f, -0.75f), vel);
            lastDirection = Direction::upRight;
            lastVec = sf::Vector2f(0.75f, -0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::D) && InputManager::getInstance().getKeyPressed(sf::Keyboard::S))
        {
            decHandler->handleMovement(Direction::rightDown, sf::Vector2f(0.75f, 0.75f), vel);
            lastDirection = Direction::rightDown;
            lastVec = sf::Vector2f(0.75f, 0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::S) && InputManager::getInstance().getKeyPressed(sf::Keyboard::A))
        {
            decHandler->handleMovement(Direction::downLeft, sf::Vector2f(-0.75f, 0.75f), vel);
            lastDirection = Direction::downLeft;
            lastVec = sf::Vector2f(-0.75f, 0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::A) && InputManager::getInstance().getKeyPressed(sf::Keyboard::W))
        {
            decHandler->handleMovement(Direction::leftUp, sf::Vector2f(-0.75f, -0.75f), vel);
            lastDirection = Direction::leftUp;
            lastVec = sf::Vector2f(-0.75f, -0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::W))
        {
            decHandler->handleMovement(lastDirection, sf::Vector2f(0, -1), vel);
            lastVec = sf::Vector2f(0, -1);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::A))
        {
            decHandler->handleMovement(Direction::left, sf::Vector2f(-1, 0), vel);
            lastDirection = Direction::left;
            lastVec = sf::Vector2f(-1, 0);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::S))
        {
            decHandler->handleMovement(lastDirection, sf::Vector2f(0, 1), vel);
            lastVec = sf::Vector2f(0, 1);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::D))
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

void MovementInputWASDCP::init()
{

}
