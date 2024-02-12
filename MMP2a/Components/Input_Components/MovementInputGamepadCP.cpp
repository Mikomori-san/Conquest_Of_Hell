//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../DecisionHandlerCP.h"
#include "MovementInputGamepadCP.h"
#include <iostream>

void MovementInputGamepadCP::update(float deltatime)
{
    if (!sf::Joystick::isConnected(m_controllerNr))
    {
        m_isControllerConnected = false;
    }
    else
    {
        if (!m_gameObject.expired())
        {
            m_vel = m_gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->getOriginalVelocity();

            if (!m_inputLock)
            {
                processInput();
            }
            else
            {
                if (m_hadInput)
                {
                    m_gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleMovement(m_lastDirection, m_lastVec, m_vel);
                }
                else
                {
                    m_gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleIdle(m_lastDirection);
                }
            }
        }
    }
}

void MovementInputGamepadCP::processInput()
{
    if (!m_gameObject.expired())
    {
        m_gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->setVelocity(0);

        std::shared_ptr<DecisionHandlerCP> decHandler = m_gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0);

        m_hadInput = true;

        sf::Vector2f joyStickLocation = InputManager::getInstance().getLeftStickPosition(m_controllerNr);
        float normLength = std::sqrt(joyStickLocation.x * joyStickLocation.x + joyStickLocation.y * joyStickLocation.y);
        sf::Vector2f joyStickLocationNormalized;
        
        if (normLength != 0.0f)
        {
            joyStickLocationNormalized = sf::Vector2f(joyStickLocation.x / normLength, joyStickLocation.y / normLength);
        }

        float length = std::sqrt(joyStickLocation.x * joyStickLocation.x + joyStickLocation.y * joyStickLocation.y);

        float adjustedVelocity = std::min(m_vel * length, m_vel);

        if (joyStickLocation.x >= 0.2f && joyStickLocation.y <= -0.2f)
        {
            decHandler->handleMovement(Direction::upRight, joyStickLocation, adjustedVelocity);
            m_lastDirection = Direction::upRight;
            m_lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x >= 0.2f && joyStickLocation.y >= 0.2f)
        {
            decHandler->handleMovement(Direction::rightDown, joyStickLocation, adjustedVelocity);
            m_lastDirection = Direction::rightDown;
            m_lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x <= -0.2f && joyStickLocation.y >= 0.2f)
        {
            decHandler->handleMovement(Direction::downLeft, joyStickLocation, adjustedVelocity);
            m_lastDirection = Direction::downLeft;
            m_lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x <= -0.2f && joyStickLocation.y <= -0.2f)
        {
            decHandler->handleMovement(Direction::leftUp, joyStickLocation, adjustedVelocity);
            m_lastDirection = Direction::leftUp;
            m_lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x < 0.2f && joyStickLocation.x > -0.2f && joyStickLocation.y < -0.2f)
        {
            decHandler->handleMovement(m_lastDirection, joyStickLocation, adjustedVelocity);
            m_lastVec = joyStickLocation;
        }
        else if (joyStickLocation.y < 0.2f && joyStickLocation.y > -0.2f && joyStickLocation.x < -0.2f)
        {
            decHandler->handleMovement(Direction::left, joyStickLocation, adjustedVelocity);
            m_lastDirection = Direction::left;
            m_lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x < 0.2f && joyStickLocation.x > -0.2f && joyStickLocation.y > 0.2f)
        {
            decHandler->handleMovement(m_lastDirection, joyStickLocation, adjustedVelocity);
            m_lastVec = joyStickLocation;
        }
        else if (joyStickLocation.y < 0.2f && joyStickLocation.y > -0.2f && joyStickLocation.x > 0.2f)
        {
            decHandler->handleMovement(Direction::right, joyStickLocation, adjustedVelocity);
            m_lastDirection = Direction::right;
            m_lastVec = joyStickLocation;
        }
        else
        {
            m_hadInput = false;
            m_gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleIdle(m_lastDirection);
        }
    }
}

void MovementInputGamepadCP::init()
{
    if (m_isControllerConnected = sf::Joystick::isConnected(m_controllerNr))
    {
        sf::Joystick::update();
    }
    else
    {
        m_isControllerConnected = false;
    }
}