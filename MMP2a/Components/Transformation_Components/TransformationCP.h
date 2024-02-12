//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Collision_Components/RigidBodyCP.h"
#include "../Component.h"
#include "../../Manager/InputManager.h"

class TransformationCP : public Component
{
public:
	TransformationCP(std::weak_ptr<GameObject> gameObject, std::string id, sf::Vector2f& pos, float rotation, float scale)
		: Component(gameObject, id), m_origin(pos), m_scale(1), m_rotation(0), m_originalPos(pos)
	{}

	virtual ~TransformationCP() = default;

	virtual void init() override;
	virtual void update(float deltaTime) override;

	virtual std::string getComponentId() override { return this->m_componentId; }
	virtual void setComponentId(std::string id) override { this->m_componentId = id; }

	virtual void setVelocity(float vel) { if (!m_velLock) { this->m_curVelocity = vel; } }
	virtual float getVelocity() { return m_curVelocity; }
	
	virtual float getOriginalVelocity() { return this->m_originalVel; }
	virtual void setOriginalVelocity(float vel) { this->m_originalVel = vel; }
	virtual void setBackupVel() { this->m_backupVel = m_originalVel; }

	virtual void setDirection(sf::Vector2f dir) { this->m_direction = dir; }
	virtual void setDirection(float x, float y) { this->m_direction = sf::Vector2f(x, y); }
	virtual sf::Vector2f& getDirection() { return m_direction; }

	virtual float getRotation() { return m_rotation; }
	virtual void setRotation(float rot) { m_rotation = rot; }

	virtual float getScale() { return m_scale; }
	virtual void setScale(float scale) { this->m_scale = scale; }

	virtual sf::Vector2f& getOrigin() { return m_origin; }
	virtual void setOrigin(sf::Vector2f ori) { m_origin = ori; }

	virtual void setPosition(sf::Vector2f newPosition) { m_position = newPosition; }
	virtual sf::Vector2f& getPosition() { return m_position; }

	bool getVelToggle() { return this->m_velLock; }

	void setOldPos() { m_position = m_oldPos - m_oldDir; m_rigid->setPos(m_position); }

	void setPosResetTimer() { this->m_posResetTimer = 0; }

	void toggleVelLock() { m_velLock = m_velLock ? false : true; }

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_origin;
	float m_curVelocity = 0;
	sf::Vector2f m_direction = sf::Vector2f(0, 0);
	float m_rotation;
	float m_scale;
	std::shared_ptr<RigidBodyCP> m_rigid;
	sf::Vector2f m_oldPos;
	sf::Vector2f m_oldDir;
	float m_originalVel;
	float m_backupVel = 0;
	int m_posResetTimer = 0;
	sf::Vector2f m_originalPos;
	bool m_velLock;
};