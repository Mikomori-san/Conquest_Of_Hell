//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "../../DebugDraw.h"
#include "../../Enums/BossAbilites.h"
#include "../Boss/BossAbility.h"
#include "../Boss/BossAttackCP.h"
#include "../Collision_Components/RectCollisionCP.h"
#include "../UI/StatusEffectCP.h"
#include "SpriteRenderCP.h"
#include <iostream>

void SpriteRenderCP::draw()
{
	for (auto& rend : m_renderComponents)
	{
		auto& sprite = rend->getSprite();
		m_window->draw(sprite);
		if (!m_gameObject.expired())
		{
			std::shared_ptr<GameObject> go = m_gameObject.lock();
			if (go->getId().find("Player") != std::string::npos)
			{
				if (go->getComponentsOfType<StatusEffectCP>().at(0)->getDisplay())
				{
					m_window->draw(go->getComponentsOfType<StatusEffectCP>().at(0)->getSprite());
				}
			}
			if (go->getId().find("Boss") != std::string::npos)
			{
				//draw abilities
				std::shared_ptr<BossAttackCP> attack = go->getComponentsOfType<BossAttackCP>().at(0);

				if (attack->getCharmInd()->getAlive())
				{
					m_window->draw(attack->getCharmInd()->getSprite());
				}
				if (attack->getMeeleeInd()->getAlive())
				{
					m_window->draw(attack->getMeeleeInd()->getSprite());
				}
				switch (attack->getAbility2()->getAbilityType())
				{
				case BossAbilites::Default:
					break;
				case BossAbilites::Melee:
					break;
				case BossAbilites::Charm:
					std::shared_ptr<CharmBA> charm = std::dynamic_pointer_cast<CharmBA>(attack->getAbility2());
					if (charm->getAlive())
					{
						m_window->draw(charm->getSprite());
					}
					break;
				}
			}
		}
	}
}

void SpriteRenderCP::update(float deltaTime)
{
	if (!m_gameObject.expired())
	{
		std::shared_ptr<GameObject> go = m_gameObject.lock();
		m_renderComponents = go->getComponentsOfType<GraphicsCP>();
	}
}

void SpriteRenderCP::init()
{
	if (!m_gameObject.expired())
	{
		std::shared_ptr<GameObject> go = m_gameObject.lock();
		m_renderComponents = go->getComponentsOfType<GraphicsCP>();
	}
}
