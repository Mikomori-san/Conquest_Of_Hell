#pragma once
#include "../../Components/Component.h"

class ScreenShakeCP : Component
{
public:
	ScreenShakeCP(std::shared_ptr<GameObject> gameObject, std::string id, std::shared_ptr<sf::RenderWindow> window, sf::View& view, float intensity, float duration, float shakeSpeed)
	: Component(gameObject, id),m_window(window), m_view(view), m_intensity(intensity), m_duration(duration), m_speed(shakeSpeed)
	{};
	void init() override {};
	void update(float deltaTime) override;
	std::string getComponentId() override { return this->componentId; }
	void setComponentId(std::string id) override { componentId = id; };
private:
	std::shared_ptr<sf::RenderWindow> m_window;
	sf::View& m_view;
	float m_intensity;
	float m_duration;
	float m_speed;
};

