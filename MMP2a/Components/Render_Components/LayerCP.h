#pragma once
#include "RenderCP.h"
#include <unordered_map>

class LayerCP : public RenderCP
{
public:
	LayerCP(std::weak_ptr<GameObject> go, std::string id, std::weak_ptr<sf::RenderWindow> renderWindow, int layerNum, std::vector<std::shared_ptr<sf::Sprite>> incLayer) : RenderCP(go, id, renderWindow, layerNum), layer(incLayer) {};
	virtual ~LayerCP() = default;

	void init() override;
	void draw() override;
	void update(float deltaTime) override;
	
	std::string getComponentId() override { return this->componentId; }
	void setComponentId(std::string id) override { this->componentId = id; }

	int getLayerNr() override { return layerNr; }
	void setLayerNr(int nr) override { layerNr = nr; }

	std::vector<std::shared_ptr<sf::Sprite>>& getLayer() { return layer; }

private:
	std::vector<std::shared_ptr<sf::Sprite>> layer;
};