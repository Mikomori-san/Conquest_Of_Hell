//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../Components/Boss/BossAttackCP.h"
#include "../Components/Boss/CharmBA.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Collision_Components/RigidBodyCP.h"
#include "../Components/Player_Components/DashCP.h"
#include "../Components/StatsCP.h"
#include "../Components/Transformation_Components/TransformationCP.h"
#include "../Enums/GamepadButton.h"
#include "PhysicsManager.h"
#include <iostream>

bool aabbVsAabb(const sf::FloatRect& a, const sf::FloatRect& b, sf::Vector2f& normal, float& penetration)
{
    auto getCenter = [](const sf::FloatRect& rect) -> sf::Vector2f
        { return sf::Vector2f(rect.left, rect.top) + 0.5F * sf::Vector2f(rect.width, rect.height); };

    const sf::Vector2f n = getCenter(b) - getCenter(a); // Vector from A to B
    const float    aExtentX = a.width * 0.5F;              // Calculate half extents along x axis
    const float    bExtentX = b.width * 0.5F;
    const float    xOverlap = aExtentX + bExtentX - abs(n.x); // Calculate overlap on x axis

    // SAT test on x axis
    if (xOverlap > 0)
    {
        const float aExtentY = a.height * 0.5F; // Calculate half extents along y axis
        const float bExtentY = b.height * 0.5F;
        const float yOverlap = aExtentY + bExtentY - abs(n.y); // Calculate overlap on y axis

        // SAT test on y axis
        if (yOverlap > 0)
        {
            // Find out which axis is axis of least penetration
            if (xOverlap < yOverlap)
            {
                // Point towards B knowing that n points from A to B
                if (n.x < 0)
                {
                    normal = sf::Vector2f(1.0F, 0.0F);
                }
                else
                {
                    normal = sf::Vector2f(-1.0F, 0.0F);
                }
                penetration = xOverlap;
                return true;
            }
            // Point towards B knowing that n points from A to B
            if (n.y < 0)
            {
                normal = sf::Vector2f(0, 1);
            }
            else
            {
                normal = sf::Vector2f(0, -1);
            }
            penetration = yOverlap;
            return true;
        }
    }
    return false;
}

void PhysicsManager::collisionCheck(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    for (auto itA = gameObjects.begin(); itA != gameObjects.end(); ++itA)
    {
        auto& body1 = *itA;

        if (body1->getComponentsOfType<RigidBodyCP>().size() == 0)
        {
            continue;
        }

        for (auto itB = itA; itB != gameObjects.end(); ++itB)
        {
            if (itA == itB)
            {
                continue;
            }

            auto& body2 = *itB;
            sf::Vector2f normal;
            float penetration = NAN;

            if (body2->getComponentsOfType<RectCollisionCP>().size() == 0)
            {
                continue;
            }

            //no collision for enemies -> the dead are unstoppable 
            if (body1->getId().find("Skeleton") != std::string::npos || body2->getId().find("Skeleton") != std::string::npos)
            {
                continue;
            }
            std::shared_ptr<RectCollisionCP> c1 = body1->getComponentsOfType<RectCollisionCP>().at(0);
            std::shared_ptr<RectCollisionCP> c2 = body2->getComponentsOfType<RectCollisionCP>().at(0);
            
            if (aabbVsAabb(
                c1->getCollisionRect(),
                c2->getCollisionRect(),
                normal,
                penetration))
            {
                std::shared_ptr<Manifold> manifold = std::make_shared<Manifold>();

                manifold->body1 = body1;
                manifold->body2 = body2;
                manifold->normal = normal;
                manifold->penetration = penetration;

                manifolds.push_back(manifold);
            }

            if (body2->getId().find("Boss") != std::string::npos)
            {
                std::shared_ptr<CharmBA> charm = std::dynamic_pointer_cast<CharmBA>(body2->getComponentsOfType<BossAttackCP>().at(0)->getAbility2());
                if (charm)
                {
                    if (body1->getId().find("Player") != std::string::npos)
                    {
                        if (auto dashes = body1->getComponentsOfType<DashCP<sf::Keyboard::Key>>(); dashes.size() != 0)
                        {
                            std::shared_ptr<DashCP<sf::Keyboard::Key>> dash = dashes.at(0);
                            if (dash && !dash->getHasIFrames() && aabbVsAabb(c1->getCollisionRect(), charm->getHitbox(), normal, penetration))
                            {
                                charm->execute();
                                charm->setDead();
                            }
                        }
                        else
                        {
                            std::shared_ptr<DashCP<GamepadButton>> dash1 = std::dynamic_pointer_cast<DashCP<GamepadButton>>(body1->getComponentsOfType<DashCP<GamepadButton>>().at(0));
                            if (dash1 && !dash1->getHasIFrames() && aabbVsAabb(c1->getCollisionRect(), charm->getHitbox(), normal, penetration))
                            {
                                charm->execute();
                                charm->setDead();
                            }
                        }
                    }
                    if (body1->getId().find("Boundary") != std::string::npos)
                    {
                        if (aabbVsAabb(c1->getCollisionRect(), charm->getHitbox(), normal, penetration))
                        {
                            charm->setDead();
                        }
                    }
                }
            }
        }
    }
}

void PhysicsManager::collisionResolve()
{
    for (auto& man : manifolds)
    {
            std::shared_ptr<GameObject> go1 = man->body1;
            std::shared_ptr<GameObject> go2 = man->body2;
            sf::Vector2f separationVector = man->normal * man->penetration;

            if (go1->getComponentsOfType<RectCollisionCP>().at(0)->isTrigger() || go2->getComponentsOfType<RectCollisionCP>().at(0)->isTrigger())
            {
                if (go1->getComponentsOfType<RigidBodyCP>().size() != 0  && go1->getId().find("Boundary") == std::string::npos)
                {
                    sf::Vector2f newPos = go1->getComponentsOfType<RigidBodyCP>().at(0)->getPos() + separationVector;
                    go1->getComponentsOfType<RigidBodyCP>().at(0)->setPos(newPos);
                    go1->getComponentsOfType<TransformationCP>().at(0)->setPosition(newPos);
                    go1->getComponentsOfType<RigidBodyCP>().at(0)->onCollision(go2);
                }
                else if(go2->getComponentsOfType<RigidBodyCP>().size() != 0 && go2->getId().find("Boundary") == std::string::npos)
                {
                    sf::Vector2f newPos = go2->getComponentsOfType<RigidBodyCP>().at(0)->getPos() + separationVector;
                    go2->getComponentsOfType<RigidBodyCP>().at(0)->setPos(newPos);
                    go2->getComponentsOfType<TransformationCP>().at(0)->setPosition(newPos);
                    go2->getComponentsOfType<RigidBodyCP>().at(0)->onCollision(go1);
                }
            }
    }
}

void PhysicsManager::update(std::vector<std::shared_ptr<GameObject>> gameObjects, float deltaTime)
{
    const float step = 0.001f;
    accumulator += deltaTime;

    while (accumulator > step)
    {
        manifolds.clear();
        collisionCheck(gameObjects);
        collisionResolve();

        accumulator -= step;
    }
}
