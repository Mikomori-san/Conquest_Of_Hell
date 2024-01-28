#pragma once
#include "InputCP.h"
#include "../../Enums/Animationtype.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"
#include "../Transformation_Components/TransformationCP.h"

class MovementInputWASDCP : public InputCP
{
public:
	MovementInputWASDCP(std::weak_ptr<GameObject> gameObject, std::string id) : InputCP(gameObject, id){}	
	~MovementInputWASDCP() = default;
	
	std::string getComponentId() override { return this->componentId; }
	void setComponentId(std::string id) { this->componentId = id; }
	void update(float deltatime) override;
	void init() override;

private:
	
	void processInput() override;
};
