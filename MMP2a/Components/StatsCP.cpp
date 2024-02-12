//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "StatsCP.h"

void StatsCP::init()
{
	m_health = m_maxHealth;
	m_damage = m_originalDamage;
	m_isDead = false;
}
