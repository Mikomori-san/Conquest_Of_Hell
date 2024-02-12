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
	std::string getId() { return componentId; }
	const BossAbilites& getAbilityType() { return abilityType; };
protected:
	std::string componentId;
	BossAbilites abilityType = Default;
};