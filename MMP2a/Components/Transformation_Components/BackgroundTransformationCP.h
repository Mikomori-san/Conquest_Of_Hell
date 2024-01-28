#pragma once
#include "TransformationCP.h"

class BackgroundTransformationCP : public TransformationCP 
{
public:
	BackgroundTransformationCP(std::weak_ptr<GameObject> gameObject, std::string id, sf::Vector2f& pos, float rotation, float scale)
		: TransformationCP(gameObject, id, pos, rotation, scale)
	{}
	virtual ~BackgroundTransformationCP() = default;

	void init() override;
	virtual void update(float deltaTime) override;

	virtual std::string getComponentId() override { return this->componentId; }
	virtual void setComponentId(std::string id) override { this->componentId = id; }

	virtual void setVelocity(float vel) override { this->curVelocity = vel; }
	virtual float getVelocity() { return curVelocity; }

	virtual void setDirection(sf::Vector2f dir) override { this->direction = dir; }
	virtual void setDirection(float x, float y) override { this->direction = sf::Vector2f(x, y); }
	virtual sf::Vector2f& getDirection() override { return direction; }

	virtual float getRotation() override { return rotation; }
	virtual void setRotation(float rot) override { rotation = rot; }

	virtual float getScale() override { return scale; }
	virtual void setScale(float scale) override { this->scale = scale; }

	virtual sf::Vector2f& getOrigin() override { return origin; }
	virtual void setOrigin(sf::Vector2f ori) override { origin = ori; }

	virtual void setPosition(sf::Vector2f newPosition) override { position = newPosition; }
	virtual sf::Vector2f& getPosition() override { return position; }
};