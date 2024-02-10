#include "stdafx.h"
#include "ScreenShakeCP.h"

void ScreenShakeCP::update(float deltaTime)
{
    timePassed += deltaTime; // Increment timePassed

    if (m_bossGraphicsCP->getAnimationType() == Attack && m_bossGraphicsCP->getAnimationFrame() == 6)
    {
        // Create a temporary view to apply screenshake
        sf::View shakenView = m_window->getView();
        sf::Vector2f original_center = shakenView.getCenter();

        // Calculate the screenshake offsets
        float shakeOffsetX = m_intensity * std::sin(m_speed * timePassed);
        float shakeOffsetY = m_intensity * std::cos(m_speed * timePassed);

        // Apply the screenshake offsets using the move function
        shakenView.move(shakeOffsetX, shakeOffsetY);

        // Set the modified view back to the window
        m_window->setView(shakenView);

 


        // Reduce the remaining duration
        m_duration -= deltaTime;

        // Check if the duration is over and reset the view's center if needed
        if (m_duration <= 0) {
            shakenView.setCenter(original_center);
            m_window->setView(shakenView);
// Reset to the original view
        }
    }
}
