//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../GameObject.h"

class MeleeIndicator
{
public:
	MeleeIndicator(std::weak_ptr<GameObject> go) : bossPtr(go) {};
	void init();
	void update(float deltaTime);

	const sf::Sprite& getSprite() { return sprite; }

	void setAnimationFrame(int frame) { animationFrame = frame; };

	void setAlive();
	bool getAlive() const { return alive; };
	void setDead();
private:
	std::weak_ptr<GameObject> bossPtr;
	sf::Sprite sprite;
	float animationTimeIndex = 0;
	float animationSpeed = 4.f;
	sf::Vector2f position;
	bool alive = false;
	int animationFrames = 9;
	int animationFrame = 0;
};

