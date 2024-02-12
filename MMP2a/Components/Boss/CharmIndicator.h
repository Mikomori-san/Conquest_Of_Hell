//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../GameObject.h"

class CharmIndicator
{
public:
	CharmIndicator(std::weak_ptr<GameObject> gameObject) : m_bossPtr(gameObject) {};
	void init() ;
	void update(float deltaTime) ;

	const sf::Sprite& getSprite() { return m_sprite; }

	void setAlive();
	bool getAlive() const { return alive; };
	void setDead();
private:
	std::weak_ptr<GameObject> m_bossPtr;
	sf::Sprite m_sprite;
	float m_animationTimeIndex = 0;
	float m_animationSpeed = 10;
	const int ANIMATION_FRAMES = 4;
	bool alive = false;
	const sf::Vector2f OFFSET = sf::Vector2f(10.f, 10.f);
};

