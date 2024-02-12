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
	MovementInputGamepadCP(std::weak_ptr<GameObject> gameObject, std::string id, int controllerNr) : InputCP(gameObject, id), m_controllerNr(controllerNr) {}

	std::string getComponentId() override { return this->m_componentId; }
	void setComponentId(std::string id) { this->m_componentId = id; }
	void update(float deltatime) override;
	void init() override;

	void toggleInputLock() override { m_inputLock = m_inputLock ? false : true; }
	int getControllerNr() { return this->m_controllerNr; }
	void setControllerNr(int nr) { this->m_controllerNr = nr; }
	bool isGamepadConnected() { return m_isControllerConnected; }
private:
	void processInput() override;
	int m_controllerNr;
	bool m_isControllerConnected;
};