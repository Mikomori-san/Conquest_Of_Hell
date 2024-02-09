#include "stdafx.h"
#include "ScreenShakeCP.h"

void ScreenShakeCP::update(float deltaTime)
{
    if (m_bossGraphicsCP->getAnimationType() == Attack && m_bossGraphicsCP->getAnimationFrame() == 6)
    {
        sf::Vector2f originalCenter = m_view.getCenter();

            // Shake the camera for the specified duration
            sf::Clock shakeTimer;
            while (shakeTimer.getElapsedTime().asSeconds() < m_duration) {
                // Calculate the displacement for the camera shake
                float offsetX = (rand() % (int)(2 * m_intensity)) - m_intensity;
                float offsetY = (rand() % (int)(2 * m_intensity)) - m_intensity;

                // Set the new center of the view
                m_view.setCenter(originalCenter.x + offsetX, originalCenter.y + offsetY);

                // Update the window
                m_window->setView(m_view);

                // Wait for a short duration to create the shaking effect
                sf::sleep(sf::seconds(1.0f / m_speed));
            }

            // Reset the view to its original position
            m_view.setCenter(originalCenter);
            m_window->setView(m_view);
    }
    
}
