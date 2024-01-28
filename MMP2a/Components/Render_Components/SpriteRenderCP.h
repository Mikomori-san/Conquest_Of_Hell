#pragma once
#include "RenderCP.h"
#include "../Graphics_Components/GraphicsCP.h"

class SpriteRenderCP : public RenderCP
{
public:
	SpriteRenderCP(std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<sf::RenderWindow> renderWindow, int layerNum) : RenderCP(gameObject, id, renderWindow, layerNum) {}
	virtual ~SpriteRenderCP() = default;

	void draw() override;
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->componentId; }
	void setComponentId(std::string id) override { this->componentId = id; }
	void init() override;
	int getLayerNr() override { return layerNr; }
	void setLayerNr(int nr) override { layerNr = nr; }
private:
	std::vector<std::shared_ptr<GraphicsCP>> renderComponents;
};