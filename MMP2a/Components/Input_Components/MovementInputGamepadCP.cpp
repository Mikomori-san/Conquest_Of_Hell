//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../DecisionHandlerCP.h"
#include "MovementInputGamepadCP.h"
#include <iostream>

void MovementInputGamepadCP::update(float deltatime)
{
    if (!sf::Joystick::isConnected(controllerNr))
    {
        isControllerConnected = false;
    }
    else
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
}

void MovementInputGamepadCP::processInput()
{
    if (!gameObject.expired())
    {
        gameObject.lock()->getComponentsOfType<TransformationCP>().at(0)->setVelocity(0);

        std::shared_ptr<DecisionHandlerCP> decHandler = gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0);

        hadInput = true;

        sf::Vector2f joyStickLocation = InputManager::getInstance().getLeftStickPosition(controllerNr);
        float normLength = std::sqrt(joyStickLocation.x * joyStickLocation.x + joyStickLocation.y * joyStickLocation.y);
        sf::Vector2f joyStickLocationNormalized;
        
        if (normLength != 0.0f)
        {
            joyStickLocationNormalized = sf::Vector2f(joyStickLocation.x / normLength, joyStickLocation.y / normLength);
        }

        float length = std::sqrt(joyStickLocation.x * joyStickLocation.x + joyStickLocation.y * joyStickLocation.y);

        float adjustedVelocity = std::min(vel * length, vel);

        if (joyStickLocation.x >= 0.2f && joyStickLocation.y <= -0.2f)
        {
            decHandler->handleMovement(Direction::upRight, joyStickLocation, adjustedVelocity);
            lastDirection = Direction::upRight;
            lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x >= 0.2f && joyStickLocation.y >= 0.2f)
        {
            decHandler->handleMovement(Direction::rightDown, joyStickLocation, adjustedVelocity);
            lastDirection = Direction::rightDown;
            lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x <= -0.2f && joyStickLocation.y >= 0.2f)
        {
            decHandler->handleMovement(Direction::downLeft, joyStickLocation, adjustedVelocity);
            lastDirection = Direction::downLeft;
            lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x <= -0.2f && joyStickLocation.y <= -0.2f)
        {
            decHandler->handleMovement(Direction::leftUp, joyStickLocation, adjustedVelocity);
            lastDirection = Direction::leftUp;
            lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x < 0.2f && joyStickLocation.x > -0.2f && joyStickLocation.y < -0.2f)
        {
            decHandler->handleMovement(lastDirection, joyStickLocation, adjustedVelocity);
            lastVec = joyStickLocation;
        }
        else if (joyStickLocation.y < 0.2f && joyStickLocation.y > -0.2f && joyStickLocation.x < -0.2f)
        {
            decHandler->handleMovement(Direction::left, joyStickLocation, adjustedVelocity);
            lastDirection = Direction::left;
            lastVec = joyStickLocation;
        }
        else if (joyStickLocation.x < 0.2f && joyStickLocation.x > -0.2f && joyStickLocation.y > 0.2f)
        {
            decHandler->handleMovement(lastDirection, joyStickLocation, adjustedVelocity);
            lastVec = joyStickLocation;
        }
        else if (joyStickLocation.y < 0.2f && joyStickLocation.y > -0.2f && joyStickLocation.x > 0.2f)
        {
            decHandler->handleMovement(Direction::right, joyStickLocation, adjustedVelocity);
            lastDirection = Direction::right;
            lastVec = joyStickLocation;
        }
        else
        {
            hadInput = false;
            gameObject.lock()->getComponentsOfType<DecisionHandlerCP>().at(0)->handleIdle(lastDirection);
        }
    }
}

void MovementInputGamepadCP::init()
{
    if (isControllerConnected = sf::Joystick::isConnected(controllerNr))
    {
        sf::Joystick::update();
    }
    else
    {
        isControllerConnected = false;
    }
}