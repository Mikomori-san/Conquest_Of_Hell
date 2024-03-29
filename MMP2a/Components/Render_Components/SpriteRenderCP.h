//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
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
	std::string getComponentId() override { return this->m_componentId; }
	void setComponentId(std::string id) override { this->m_componentId = id; }
	void init() override;
	int getLayerNr() override { return m_layerNr; }
	void setLayerNr(int nr) override { m_layerNr = nr; }
	std::weak_ptr<GameObject> getGO() override { return m_gameObject; }


private:
	std::vector<std::shared_ptr<GraphicsCP>> m_renderComponents;
};