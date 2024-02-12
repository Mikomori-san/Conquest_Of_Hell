//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Graphics_Components/GraphicsCP.h"
#include "../Component.h"

class StatusEffectCP : public Component
{
public:
	StatusEffectCP(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture) : Component(gameObject, id), sprite(sf::Sprite(texture)) {};

	void init() override;
	void update(float deltaTime) override;
	void toggleDisplayStatus() { display = !display; };
	bool getDisplay() { return display; };
	const sf::Sprite& getSprite() { return sprite; };

	std::string getComponentId() override { return componentId; };
	void setComponentId(std::string id) override { componentId = id; };
private:
	int offsetX = 20;
	int offsetY = 20;
	bool display = false;
	sf::Sprite sprite;
};