#pragma once
#include "../Component.h"
#include "../../Manager/InputManager.h"
#include "../Collision_Components/RigidBodyCP.h"


class TransformationCP : public Component
{
public:
	TransformationCP(std::weak_ptr<GameObject> gameObject, std::string id, sf::Vector2f& pos, float rotation, float scale)
		: Component(gameObject, id), position(pos), origin(pos), scale(1), rotation(0)
	{}

	virtual ~TransformationCP() = default;

	virtual void init() override;
	virtual void update(float deltaTime) override;

	virtual std::string getComponentId() override { return this->componentId; }
	virtual void setComponentId(std::string id) override { this->componentId = id; }

	virtual void setVelocity(float vel) { this->curVelocity = vel; }
	virtual float getVelocity() { return curVelocity; }
	
	virtual float getOriginalVelocity() { return this->originalVel; }
	virtual void setOriginalVelocity(float vel) { this->originalVel = vel; }
	virtual void setBackupVel() { this->backupVel = originalVel; }

	virtual void setDirection(sf::Vector2f dir) { this->direction = dir; }
	virtual void setDirection(float x, float y) { this->direction = sf::Vector2f(x, y); }
	virtual sf::Vector2f& getDirection() { return direction; }

	virtual float getRotation() { return rotation; }
	virtual void setRotation(float rot) { rotation = rot; }

	virtual float getScale() { return scale; }
	virtual void setScale(float scale) { this->scale = scale; }

	virtual sf::Vector2f& getOrigin() { return origin; }
	virtual void setOrigin(sf::Vector2f ori) { origin = ori; }

	virtual void setPosition(sf::Vector2f newPosition) { position = newPosition; }
	virtual sf::Vector2f& getPosition() { return position; }

	void setOldPos() { position = oldPos - oldDir; rigid->setPos(position); }

	void setPosResetTimer() { this->posResetTimer = 0; }

protected:
	sf::Vector2f position;
	sf::Vector2f origin;
	float curVelocity = 0;
	sf::Vector2f direction = sf::Vector2f(0, 0);
	float rotation;
	float scale;
	std::shared_ptr<RigidBodyCP> rigid;
	sf::Vector2f oldPos;
	sf::Vector2f oldDir;
	float originalVel;
	float backupVel = 0;
	int posResetTimer = 0;
};