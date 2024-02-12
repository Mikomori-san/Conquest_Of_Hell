//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "BossAbility.h"
#include "../../GameObject.h"

class CharmBA : public BossAbility
{
public:
	CharmBA(std::string id, std::weak_ptr<GameObject> player) : BossAbility(id), playerPtr(player) {};
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
	void uncharmPlayer();
	std::weak_ptr<GameObject> playerPtr;
	sf::Sprite sprite;
	sf::FloatRect hitbox;
	float ttl = 10;
	float moveSpeed = 1;
	float animationTimeIndex = 0;
	float animationSpeed = 10;
	sf::Vector2f position;
	sf::Vector2f direction;
	bool alive = false;
	bool charmed = false;
	float charmDuration = 1;
};