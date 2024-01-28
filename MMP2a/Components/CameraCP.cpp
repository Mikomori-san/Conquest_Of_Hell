#include "stdafx.h"
#include "CameraCP.h"
#include "Transformation_Components/CameraTransformationCP.h"

void CameraCP::init()
{
	if (std::shared_ptr<GameObject> tempP = gameObject.lock()) //check if Gameobject is nullptr
    {
        transform = tempP->getComponentsOfType<CameraTransformationCP>().at(0);
        view.reset(sf::FloatRect(transform->getPosition(), size));
    }
}

void CameraCP::update(float deltaTime)
{
    if (std::shared_ptr<GameObject> tempP = gameObject.lock())
    {
        view.setCenter(transform->getPosition());
    }

    window->setView(view);
}
