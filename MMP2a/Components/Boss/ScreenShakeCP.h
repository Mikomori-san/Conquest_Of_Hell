//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../../Components/Component.h"
#include "../../Enums/Boss_Animationtype.h"
#include "../Graphics_Components/AnimatedGraphicsCP.h"

class ScreenShakeCP : public Component
{
public:
    ScreenShakeCP(std::shared_ptr<AnimatedGraphicsCP<Boss_Animationtype>> bossGraphicsCP, std::shared_ptr<GameObject> gameObject, std::string id, std::shared_ptr<sf::RenderWindow> window)
        : Component(gameObject, id), m_bossGraphicsCP(bossGraphicsCP), m_window(window)
    {}

    void init() override;
    void update(float deltaTime) override;
    std::string getComponentId() override { return this->m_componentId; }
    void setComponentId(std::string id) override { m_componentId = id; };
    void updateScreen(sf::View& view) { this->m_originalView = view; }
private:
    std::shared_ptr<AnimatedGraphicsCP<Boss_Animationtype>> m_bossGraphicsCP;
    std::shared_ptr<sf::RenderWindow> m_window;
    
    sf::View m_originalView;
    float m_intensity;
    float m_speed;
    float timePassed;
    const int ATTACK_FRAME = 6;
    const int ATTACK_FRAME_BEFORE_HIT = 3;
    const float SHAKE_DURATION_THRESHOLD = 0.5f;
    bool setScreenShake;
};
