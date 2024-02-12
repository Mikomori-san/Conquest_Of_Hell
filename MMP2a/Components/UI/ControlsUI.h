//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Graphics_Components/GraphicsCP.h"

class ControlsUI : public GraphicsCP
{
public:
	ControlsUI(std::weak_ptr<GameObject> gameObject, std::string id, const sf::Texture& texture, std::weak_ptr<sf::RenderWindow> window) : GraphicsCP(gameObject, id, texture), 
	m_window(window){}
	void init() override;
	void update(float delaTime) override;
private:
	std::weak_ptr<sf::RenderWindow> m_window;
	sf::Vector2f offset = sf::Vector2f(10.f,10.f);
};