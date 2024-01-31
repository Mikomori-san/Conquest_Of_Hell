#include "stdafx.h"
#include "SpawnerCP.h"
#include <iostream>


void SpawnerCP::init()
{
}

void SpawnerCP::spawnEnemy()
{


    

}



void SpawnerCP::update(float deltaTime)
{
    spawnTimer += deltaTime;


    if (m_enemyIdList.size() < m_maxEnemies)
    {
        if (spawnTimer >= m_spawnTime && m_enemyIdList.size() == 0)
        {
            m_enemyIdList.push_back(0);

            std::cout << "------------------------------enemy1 spawned-------" << 0 << std::endl;

            spawnFirstEnemy = true;
            m_gameObjects.push_back(m_enemy);
            spawnTimer = 0.0f;

        }
        if (spawnTimer >= m_spawnTime && m_enemyIdList.size() == 1)
        {
            m_enemyIdList.push_back(1);


            std::cout << "------------------------------enemy2 spawned-------" << 1 << std::endl;
            spawnSecondEnemy = true;

            

            spawnTimer = 0.0f;
        }
    }
}
