//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "BossAbility.h"
#include "../../GameObject.h"

class CharmBA : public BossAbility
{
public:
	CharmBA(std::string id, std::weak_ptr<GameObject> player) : BossAbility(id), m_playerPtr(player) {};
	void init() override;
	void update(float deltaTime) override;
	void execute() override;

	void setPosition(const sf::Vector2f& vec) { m_position = vec; };
	void setDirection(const sf::Vector2f& vec) { m_direction = vec; };
	const sf::Vector2f& getPosition() { return m_sprite.getPosition(); }
	const sf::Sprite& getSprite() { return m_sprite; }

	const sf::FloatRect& getHitbox();

	void setAlive();
	bool getAlive() const { return alive; };
	void setDead();
private:
	void charmPlayer();
	void uncharmPlayer();
	std::weak_ptr<GameObject> m_playerPtr;
	sf::Sprite m_sprite;
	sf::FloatRect m_hitbox;
	float m_ttl = 10;
	const float MOVE_SPEED = 1;
	float animationTimeIndex = 0;
	float animationSpeed = 10;
	sf::Vector2f m_position;
	sf::Vector2f m_direction;
	bool alive = false;
	bool charmed = false;
	float m_charmDuration = 1;
};