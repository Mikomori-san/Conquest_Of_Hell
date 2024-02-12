//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../Enums/Player_Animationtype.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Transformation_Components/TransformationCP.h"
#include "InputCP.h"

class MovementInputWASDCP : public InputCP
{
public:
	MovementInputWASDCP(std::weak_ptr<GameObject> gameObject, std::string id) : InputCP(gameObject, id){}	
	~MovementInputWASDCP() = default;
	
	std::string getComponentId() override { return this->m_componentId; }
	void setComponentId(std::string id) { this->m_componentId = id; }
	void update(float deltatime) override;
	void init() override;

	void toggleInputLock() override { inputLock = inputLock ? false : true; }

private:
	void processInput() override;
};
