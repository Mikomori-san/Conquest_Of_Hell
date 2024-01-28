#pragma once
#include "../Component.h"

class RenderCP : public Component
{
public:
	RenderCP(std::weak_ptr<GameObject> gameObject, std::string id, std::weak_ptr<sf::RenderWindow> renderWindow, int incLayerNr) : Component(gameObject, id), window(renderWindow), layerNr(incLayerNr) {}
	virtual ~RenderCP() = default;

	virtual void draw() = 0;
	virtual void update (float deltaTime) override = 0;
	virtual std::string getComponentId() override = 0;
	virtual void setComponentId(std::string id) override = 0;
	virtual void init() override = 0;
	virtual int getLayerNr() = 0;
	virtual void setLayerNr(int nr) = 0;
protected:
	std::shared_ptr<sf::RenderWindow> window;
	int layerNr;
};