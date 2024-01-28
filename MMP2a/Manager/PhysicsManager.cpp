#include "stdafx.h"
#include "PhysicsManager.h"
#include "../Components/Collision_Components/RectCollisionCP.h"
#include "../Components/Collision_Components/RigidBodyCP.h"
#include <iostream>

void positionalCorrection(Manifold& man)
{
    const float percent = 0.2f;
    const float slop = 0.01f;

    std::shared_ptr<RigidBodyCP> r1 = man.body1->getComponentsOfType<RigidBodyCP>().at(0);
    std::shared_ptr<RigidBodyCP> r2 = man.body2->getComponentsOfType<RigidBodyCP>().at(0);

    sf::Vector2f correction = std::max(man.penetration - slop, 0.0f) / (r1->getInvMass() + r2->getInvMass()) * percent * man.normal;

    r2->setPosNotifyTransf(r1->getPos() + r1->getInvMass() * correction);
    r2->setPosNotifyTransf(r2->getPos() + r2->getInvMass() * correction);
}

bool aabbVsAabb(const sf::FloatRect& a, const sf::FloatRect& b, sf::Vector2f& normal, float& penetration)
{
    auto getCenter = [](const sf::FloatRect& rect) -> sf::Vector2f
        { return sf::Vector2f(rect.left, rect.top) + 0.5F * sf::Vector2f(rect.width, rect.height); };

    // TODO: implement SAT for axis aligned bounding box (AABB) collision
    // TODO determine separation normal for collision resolution, i.e., the axis
    // showing the smallest penetration.
    // TODO determine the corresponding penetration depth for the positional correction step.

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
        }
    }
}

void PhysicsManager::collisionResolve()
{
    for (auto& man : manifolds)
    {
            std::shared_ptr<GameObject> go1 = man->body1;
            std::shared_ptr<GameObject> go2 = man->body2;
            
            // Collisions against Players are not being detected
            if (go1->getId().find("Player") != std::string::npos && go2->getId().find("Player") != std::string::npos)
            {
                continue;
            }

            if (go1->getComponentsOfType<RectCollisionCP>().at(0)->isTrigger() || go2->getComponentsOfType<RectCollisionCP>().at(0)->isTrigger())
            {
                std::cout << "Collision" << std::endl;
                if (go1->getComponentsOfType<RigidBodyCP>().size() != 0)
                {
                    // Send Notification to Rigid Body of go1
                    go1->getComponentsOfType<RigidBodyCP>().at(0)->onCollision(go2);
                }
                else if(go2->getComponentsOfType<RigidBodyCP>().size() != 0)
                {
                    // Send Notification to Rigid Body of go2
                    go2->getComponentsOfType<RigidBodyCP>().at(0)->onCollision(go1);
                }
            }
            else
            {

                // Rigid Body Logic

                /*
                std::shared_ptr<RigidBodyCP> r1 = go1->getComponentsOfType<RigidBodyCP>().at(0);
                std::shared_ptr<RigidBodyCP> r2 = go2->getComponentsOfType<RigidBodyCP>().at(0);

                const sf::Vector2f rv = r1->getVel() - r2->getVel();

                const float velAlongNormal = rv.x * man->normal.x + rv.y * man->normal.y;

                if (velAlongNormal >= 0) // > 0
                {
                    return;
                }

                if (bool restitutionOn = true)
                {
                    sf::Vector2f impulse = velAlongNormal * man->normal;

                    r1->setVelNotifyTransf(r1->getVel() - 0.5f * impulse);
                    r2->setVelNotifyTransf(r2->getVel() + 0.5f * impulse);
                }

                if (bool posCorrection = false)
                {
                    positionalCorrection(*man);
                }
                */
                
            }
    }
}

void PhysicsManager::update(std::vector<std::shared_ptr<GameObject>> gameObjects, float deltaTime)
{
    const float step = 1.f / 100.f;
    accumulator += deltaTime;

    while (accumulator > step)
    {
        manifolds.clear();
        collisionCheck(gameObjects);
        collisionResolve();

        accumulator -= step;
    }
}
