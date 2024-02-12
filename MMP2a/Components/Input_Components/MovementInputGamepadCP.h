//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Collision_Components/RigidBodyCP.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "InputCP.h"
#include "../../Enums/Player_Animationtype.h"
#include "../Transformation_Components/TransformationCP.h"

class MovementInputGamepadCP : public InputCP
{
public:
	MovementInputGamepadCP(std::weak_ptr<GameObject> gameObject, std::string id, int controllerNr) : InputCP(gameObject, id), controllerNr(controllerNr) {}

	std::string getComponentId() override { return this->m_componentId; }
	void setComponentId(std::string id) { this->m_componentId = id; }
	void update(float deltatime) override;
	void init() override;

	void toggleInputLock() override { inputLock = inputLock ? false : true; }
	int getControllerNr() { return this->controllerNr; }
	void setControllerNr(int nr) { this->controllerNr = nr; }
	bool isGamepadConnected() { return isControllerConnected; }
private:
	void processInput() override;
	int controllerNr;
	bool isControllerConnected;
};