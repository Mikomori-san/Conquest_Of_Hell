#pragma once
#include "TransformationCP.h"

class CameraTransformationCP : public TransformationCP
{
public:
	CameraTransformationCP(std::weak_ptr<GameObject> gameObject, std::string id, sf::Vector2f& pos, float rotation, float scale)
		: TransformationCP(gameObject, id, pos, rotation, scale)
	{}

	void init() override;
	virtual void update(float deltaTime) override;

	std::string getComponentId() override { return this->componentId; }
	void setComponentId(std::string id) override { this->componentId = id; }

	void setVelocity(float vel) { this->curVelocity = vel; }
	float getVelocity() { return curVelocity; }

	void setDirection(sf::Vector2f dir) { this->direction = dir; }
	void setDirection(float x, float y) { this->direction = sf::Vector2f(x, y); }
	sf::Vector2f& getDirection() { return direction; }

	float getRotation() { return rotation; }
	void setRotation(float rot) { rotation = rot; }

	float getScale() { return scale; }
	void setScale(float scale) { this->scale = scale; }

	sf::Vector2f& getOrigin() { return origin; }
	void setOrigin(sf::Vector2f ori) { origin = ori; }

	void setPosition(sf::Vector2f newPosition) { position = newPosition; }
	sf::Vector2f& getPosition() { return position; }

};