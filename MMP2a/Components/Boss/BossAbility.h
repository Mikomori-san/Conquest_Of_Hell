//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../Enums/BossAbilites.h"
#include "../../GameObject.h"

class BossAbility
{
public:
	BossAbility(std::string id){}
	virtual void init() {};
	virtual void update(float deltaTime) {};
	virtual void execute() {};
	std::string getId() { return m_componentId; }
	const BossAbilites& getAbilityType() { return m_abilityType; };
protected:
	std::string m_componentId;
	BossAbilites m_abilityType = Default;
};