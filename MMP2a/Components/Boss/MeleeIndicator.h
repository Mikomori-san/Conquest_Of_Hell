//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../GameObject.h"

class MeleeIndicator
{
public:
	MeleeIndicator(std::weak_ptr<GameObject> go) : m_bossPtr(go) {};
	void init();
	void update(float deltaTime);

	const sf::Sprite& getSprite() { return m_sprite; }

	void setAnimationFrame(int frame) { animationFrame = frame; };

	void setAlive();
	bool getAlive() const { return m_alive; };
	void setDead();
private:
	std::weak_ptr<GameObject> m_bossPtr;
	sf::Sprite m_sprite;
	float m_animationTimeIndex = 0;
	float m_animationSpeed = 4.f;
	sf::Vector2f m_position;
	bool m_alive = false;
	const int ANIMATION_FRAMES = 9;
	int animationFrame = 0;
};

