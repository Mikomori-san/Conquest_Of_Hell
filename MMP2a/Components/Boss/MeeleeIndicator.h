#pragma once
class MeeleeIndicator
{
public:
	MeeleeIndicator() {};
	void init();
	void update(float deltaTime);


	void setPosition(const sf::Vector2f& vec) { position = vec; };
	const sf::Vector2f& getPosition() { return sprite.getPosition(); }
	const sf::Sprite& getSprite() { return sprite; }

	const sf::FloatRect& getHitbox() { return hitbox; };

	void setAlive();
	bool getAlive() const { return alive; };
	void setDead();
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::FloatRect hitbox;
	float ttl = 2.f;
	float animationTimeIndex = 0;
	float animationSpeed = 6.f;
	sf::Vector2f position;
	bool alive = false;

};

