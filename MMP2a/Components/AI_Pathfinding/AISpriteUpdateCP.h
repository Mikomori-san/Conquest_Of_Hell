#pragma once
#include "../Component.h"
#include "../../Enums/Player_Animationtype.h"

class AISpriteUpdateCP : public Component
{
public:
	AISpriteUpdateCP(std::weak_ptr<GameObject> incGameObject, std::string id) : Component(incGameObject, id) {};
	void update(float deltaTime) override;
	std::string getComponentId() override;
	void setComponentId(std::string id) override;
	void init() override;

private:
	Player_Animationtype lastAnimation = Player_Animationtype::RightIdle;
};