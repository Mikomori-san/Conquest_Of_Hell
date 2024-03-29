//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "RenderCP.h"
#include <unordered_map>

class LayerCP : public RenderCP
{
public:
	LayerCP(std::weak_ptr<GameObject> go, std::string id, std::weak_ptr<sf::RenderWindow> renderWindow, int layerNum, std::vector<std::shared_ptr<sf::Sprite>> incLayer) : RenderCP(go, id, renderWindow, layerNum), m_layer(incLayer) {};
	virtual ~LayerCP() = default;

	void init() override {};
	void draw() override;
	void update(float deltaTime) override {};
	
	std::string getComponentId() override { return this->m_componentId; }
	void setComponentId(std::string id) override { this->m_componentId = id; }

	int getLayerNr() override { return m_layerNr; }
	void setLayerNr(int nr) override { m_layerNr = nr; }

	std::vector<std::shared_ptr<sf::Sprite>>& getLayer() { return m_layer; }

	std::weak_ptr<GameObject> getGO() override { return m_gameObject; }

private:
	std::vector<std::shared_ptr<sf::Sprite>> m_layer;
};