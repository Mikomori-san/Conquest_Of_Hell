//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../DecisionHandlerCP.h"
#include <iostream>
#include "MovementInputWASDCP.h"

void MovementInputWASDCP::update(float deltatime)
{
    if (!gameObject.expired())
    {
        m_vel = gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->getOriginalVelocity();

        if (!m_inputLock)
        {
            processInput();
        }
        else
        {
            if (m_hadInput)
            {
                gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleMovement(m_lastDirection, m_lastVec, m_vel);
            }
            else
            {
                gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleIdle(m_lastDirection);
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

        m_hadInput = true;

        if (InputManager::getInstance().getKeyPressed(sf::Keyboard::W) && InputManager::getInstance().getKeyPressed(sf::Keyboard::D))
        {
            decHandler->handleMovement(Direction::upRight, sf::Vector2f(0.75f, -0.75f), m_vel);
            m_lastDirection = Direction::upRight;
            m_lastVec = sf::Vector2f(0.75f, -0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::D) && InputManager::getInstance().getKeyPressed(sf::Keyboard::S))
        {
            decHandler->handleMovement(Direction::rightDown, sf::Vector2f(0.75f, 0.75f), m_vel);
            m_lastDirection = Direction::rightDown;
            m_lastVec = sf::Vector2f(0.75f, 0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::S) && InputManager::getInstance().getKeyPressed(sf::Keyboard::A))
        {
            decHandler->handleMovement(Direction::downLeft, sf::Vector2f(-0.75f, 0.75f), m_vel);
            m_lastDirection = Direction::downLeft;
            m_lastVec = sf::Vector2f(-0.75f, 0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::A) && InputManager::getInstance().getKeyPressed(sf::Keyboard::W))
        {
            decHandler->handleMovement(Direction::leftUp, sf::Vector2f(-0.75f, -0.75f), m_vel);
            m_lastDirection = Direction::leftUp;
            m_lastVec = sf::Vector2f(-0.75f, -0.75f);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::W))
        {
            decHandler->handleMovement(m_lastDirection, sf::Vector2f(0, -1), m_vel);
            m_lastVec = sf::Vector2f(0, -1);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::A))
        {
            decHandler->handleMovement(Direction::left, sf::Vector2f(-1, 0), m_vel);
            m_lastDirection = Direction::left;
            m_lastVec = sf::Vector2f(-1, 0);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::S))
        {
            decHandler->handleMovement(m_lastDirection, sf::Vector2f(0, 1), m_vel);
            m_lastVec = sf::Vector2f(0, 1);
        }
        else if (InputManager::getInstance().getKeyPressed(sf::Keyboard::D))
        {
            decHandler->handleMovement(Direction::right, sf::Vector2f(1, 0), m_vel);
            m_lastDirection = Direction::right;
            m_lastVec = sf::Vector2f(1, 0);
        }
        else
        {
            m_hadInput = false;
            gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleIdle(m_lastDirection);
        }
    }
}

void MovementInputWASDCP::init()
{

}
