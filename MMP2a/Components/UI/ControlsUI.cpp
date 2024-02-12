//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "ControlsUI.h"

void ControlsUI::init()
{
	if (!m_window.expired())
	{
		std::shared_ptr<sf::RenderWindow> window = m_window.lock();
		m_sprite->setScale(0.1f, 0.1f);
		m_sprite->setPosition(window->getView().getCenter() - (window->getView().getSize() / 2.f) + offset);
	}
}

void ControlsUI::update(float delaTime)
{
	if (!m_window.expired())
	{
		std::shared_ptr<sf::RenderWindow> window = m_window.lock();
		m_sprite->setPosition(window->getView().getCenter() - (window->getView().getSize() / 2.f) + offset);
	}
}
