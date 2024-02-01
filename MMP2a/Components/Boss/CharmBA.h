#pragma once
#include "BossAbility.h"
#include "../../GameObject.h"

class CharmBA : public BossAbility
{
public:
	CharmBA(std::string id) : BossAbility(id) {};
	void init() override;
	void update(float deltaTime) override;
	void execute() override;

	void setPosition(const sf::Vector2f& vec) { position = vec; };
	void setDirection(const sf::Vector2f& vec) { direction = vec; };
	const sf::Vector2f& getPosition() { return sprite.getPosition(); }
	const sf::Sprite& getSprite() { return sprite; }

	const sf::FloatRect& getHitbox();

	void setAlive();
	bool getAlive() const { return alive; };
	void setDead();
private:
	void charmPlayer();
	sf::Sprite sprite;
	sf::FloatRect hitbox;
	float ttl = 3;
	float moveSpeed = 1;
	float animationTimeIndex = 0;
	float animationSpeed = 10;
	sf::Vector2f position;
	sf::Vector2f direction;
	bool alive = false;
};