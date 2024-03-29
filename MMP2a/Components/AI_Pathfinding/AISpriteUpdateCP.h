//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Component.h"
#include "../../Enums/Enemy_Animationtype.h"

class AISpriteUpdateCP : public Component
{
public:
	AISpriteUpdateCP(std::weak_ptr<GameObject> incGameObject, std::string id) : Component(incGameObject, id) {};
	void update(float deltaTime) override;
	std::string getComponentId() override;
	void setComponentId(std::string id) override;
	void init() override;

private:
	Enemy_Animationtype m_lastAnimation = Enemy_Animationtype::IdleRight;
};