#include "stdafx.h"
#include "SpawnerCP.h"
#include <iostream>
#include "../Render_Components/SpriteRenderCP.h"


void SpawnerCP::init()
{
}

void SpawnerCP::spawnEnemy()
{


    

}



void SpawnerCP::update(float deltaTime)
{
    spawnTimer += deltaTime;


    if (spawnTimer >= m_spawnTime && m_enemyIdList.size() < m_maxEnemies)
    {
        if (m_enemyIdList.empty())
        {
            m_enemyIdList.push_back(0);

            std::cout << "------------------------------enemy1 spawned-------" << 0 << std::endl;

            m_gameObjects.push_back(m_enemy);
            for (auto& comps : m_enemy->getComponents())
            {
                comps->init();
            }
            spawnTimer = 0.0f;
        }
       
        if (m_enemyIdList.size() == 1)
        {
            m_enemyIdList.push_back(1);


            std::cout << "------------------------------enemy2 spawned-------" << 1 << std::endl;
            m_gameObjects.push_back(m_enemy2);
            for (auto& comps : m_enemy2->getComponents())
            {
                comps->init();
            }

            spawnTimer = 0.0f;
        }
    }
}
