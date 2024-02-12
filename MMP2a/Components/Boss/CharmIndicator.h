#pragma once
#include "../../GameObject.h"
class CharmIndicator
{
public:
	CharmIndicator(std::weak_ptr<GameObject> gameObject) : bossPtr(gameObject) {};
	void init() ;
	void update(float deltaTime) ;

	const sf::Sprite& getSprite() { return sprite; }

	void setAlive();
	bool getAlive() const { return alive; };
	void setDead();
private:
	std::weak_ptr<GameObject> bossPtr;
	sf::Sprite sprite;
	float animationTimeIndex = 0;
	float animationSpeed = 10;
	int animationFrames = 4;
	bool alive = false;
	sf::Vector2f offset = sf::Vector2f(10.f, 10.f);
};

