//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "Transformation_Components/CameraTransformationCP.h"
#include "CameraCP.h"

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
