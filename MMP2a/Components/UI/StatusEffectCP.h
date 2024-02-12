//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Graphics_Components/GraphicsCP.h"
#include "../Component.h"

class StatusEffectCP : public Component
{
public:
	StatusEffectCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture) : Component(gameObject, id), m_sprite(sf::Sprite(texture)) {};

	void init() override;
	void update(float deltaTime) override;
	void toggleDisplayStatus() { m_display = !m_display; };
	bool getDisplay() { return m_display; };
	const sf::Sprite& getSprite() { return m_sprite; };

	std::string getComponentId() override { return m_componentId; };
	void setComponentId(std::string id) override { m_componentId = id; };
private:
	const int m_offsetX = 20;
	const int m_offsetY = 20;
	bool m_display = false;
	sf::Sprite m_sprite;
};