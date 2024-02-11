#include "stdafx.h"
#include "ScreenShakeCP.h"

void ScreenShakeCP::update(float deltaTime)
{
    timePassed += deltaTime; 

    if (m_bossGraphicsCP->getAnimationType() == Attack && m_bossGraphicsCP->getAnimationFrame() == 6)
    {
        sf::View shakenView = m_window->getView();
        sf::Vector2f original_center = shakenView.getCenter();

        float shakeOffsetX = m_intensity * std::sin(m_speed * timePassed);
        float shakeOffsetY = m_intensity * std::cos(m_speed * timePassed);

        shakenView.move(shakeOffsetX, shakeOffsetY);

        m_window->setView(shakenView);

        m_duration -= deltaTime;

        if (m_duration <= 0) {
            shakenView.setCenter(original_center);
            m_window->setView(shakenView);

        }
    }
}
