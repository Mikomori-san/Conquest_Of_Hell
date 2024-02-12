//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include <iostream>
#include "../AI_Pathfinding/AStarCP.h"
#include "../AI_Pathfinding/AISpriteUpdateCP.h"
#include "../AI_Pathfinding/ControllerCP.h"
#include "../AI_Pathfinding/SteeringCP.h"
#include "../Enemy_Components/EnemyAttackCP.h"
#include "../Input_Components/MovementInputGamepadCP.h"
#include "../Player_Components/PlayerAttackCP.h"
#include "../Render_Components/SpriteRenderCP.h"
#include "../StatsCP.h"
#include "SpawnerCP.h"

void SpawnerCP::late_init_if_enemy_spawned(std::shared_ptr<GameObject> player)
{
    for (auto& enemy : availableEnemies)
    {
        if (player->getComponentsOfType<MovementInputGamepadCP>().size() != 0)
        {
            std::shared_ptr<PlayerAttackCP<GamepadButton>> playerAttackCP = player->getComponentsOfType<PlayerAttackCP<GamepadButton>>().at(0);
            playerAttackCP->addEnemy(enemy);
        }
        else
        {
            std::shared_ptr<PlayerAttackCP<sf::Keyboard::Key>> playerAttackCP = player->getComponentsOfType<PlayerAttackCP<sf::Keyboard::Key>>().at(0);
            playerAttackCP->addEnemy(enemy);
        }

        std::vector<std::weak_ptr<GameObject>> players;
        for (auto& go1 : m_gameObjects)
        {
            if (go1->getId().find("Player") != std::string::npos)
            {
                players.push_back(go1);
            }
        }

        std::shared_ptr<ControllerCP> enemyAIController = std::make_shared<ControllerCP>(enemy, "EnemyControllerCP", players);
        enemy->addComponent(enemyAIController);

        std::shared_ptr<AStarCP> enemyAStarCP = std::make_shared<AStarCP>(enemy, "EnemyAStarCP", std::vector<std::vector<int>>(aStarGridSize.x, std::vector<int>(aStarGridSize.y, 0)), unMovablePositions, sf::Vector2f(0, 0), mapTileSize);
        enemy->addComponent(enemyAStarCP);

        std::shared_ptr<SteeringCP> enemySteeringCP = std::make_shared<SteeringCP>(enemy, "EnemySteeringCP");
        enemy->addComponent(enemySteeringCP);

        std::shared_ptr<AISpriteUpdateCP> enemyAISpriteUpdateCP = std::make_shared<AISpriteUpdateCP>(enemy, "EnemyAISpriteUpdateCP");
        enemy->addComponent(enemyAISpriteUpdateCP);

        std::shared_ptr<EnemyAttackCP> enemyAttackCP = std::make_shared<EnemyAttackCP>(enemy, "EnemyAttackCP", players.at(0), 200);
        enemy->addComponent(enemyAttackCP);
    }
}


void SpawnerCP::init()
{

    std::shared_ptr<GameObject> player;
    
    for (auto& go : m_gameObjects)
    {
        if (go->getId().find("Player") != std::string::npos)
        {
            player = go;
            break;
        }
    }

    late_init_if_enemy_spawned(player);
    spawnTimer = 0;
}

void SpawnerCP::spawnEnemy()
{

}



void SpawnerCP::update(float deltaTime)
{
    for (auto& enemy : availableEnemies)
    {
        if (enemy1Spawned && enemy->getId().find("0") != std::string::npos)
        {
            if (enemy->getComponentsOfType<StatsCP>().at(0)->getHealth() <= 0)
            {
                enemy1Spawned = false;
                std::cout << "Enemy 1 is not spawned" << std::endl;
            }
        }
        if (enemy2Spawned && enemy->getId().find("1") != std::string::npos)
        {
            if (enemy->getComponentsOfType<StatsCP>().at(0)->getHealth() <= 0)
            {
                enemy2Spawned = false;
                std::cout << "enemy 2 is not spawned" << std::endl;
            }
        }
    }

    if (spawnTimer >= m_spawnTime)
    {
        if (!enemy1Spawned)
        {
            enemy1Spawned = true;

            std::cout << "------------------------------enemy1 spawned-------" << 0 << std::endl;

            m_gameObjects.push_back(availableEnemies.at(0));

            for (auto& comps : availableEnemies.at(0)->getComponents())
            {
                comps->init();
            }
        } 
        else if (!enemy2Spawned)
        {
            enemy2Spawned = true;

            std::cout << "------------------------------enemy2 spawned-------" << 1 << std::endl;

            m_gameObjects.push_back(availableEnemies.at(1));

            for (auto& comps : availableEnemies.at(1)->getComponents())
            {
                comps->init();
            }
        }
        spawnTimer = 0.0f;
    }
    spawnTimer += deltaTime;
}
