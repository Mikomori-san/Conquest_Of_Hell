#pragma once
#include "../../Components/Component.h"
#include "../../Enums/Boss_Animationtype.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"

class ScreenShakeCP : public Component
{
public:
    ScreenShakeCP(std::shared_ptr<AnimatedGraphicsCP<Boss_Animationtype>> bossGraphicsCP, std::shared_ptr<GameObject> gameObject, std::string id, std::shared_ptr<sf::RenderWindow> window,
         float intensity, float duration, float shakeSpeed)
        : Component(gameObject, id), m_bossGraphicsCP(bossGraphicsCP), m_window(window),
       m_intensity(intensity), m_duration(duration), m_speed(shakeSpeed)
    {}

    void init() override {};
    void update(float deltaTime) override;
    std::string getComponentId() override { return this->componentId; }
    void setComponentId(std::string id) override { componentId = id; };
private:
    std::shared_ptr<AnimatedGraphicsCP<Boss_Animationtype>> m_bossGraphicsCP;
    std::shared_ptr<sf::RenderWindow> m_window;
    
   const sf::View& m_originalView = m_window->getView();
    float m_intensity;
    float m_duration;
    float m_speed;
    float timePassed;
};
