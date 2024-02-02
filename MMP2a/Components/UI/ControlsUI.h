#pragma once
#include "../Graphics_Components/GraphicsCP.h"

class ControlsUI : public GraphicsCP
{
public:
	ControlsUI(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture) : GraphicsCP(gameObject, id, texture){}
	void init() override;
};