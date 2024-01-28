#pragma once
#include "Component.h"
#include "../Components/Transformation_Components/TransformationCP.h";
#include <memory>

class GameObject;

class CameraCP : public Component
{
public:
	CameraCP(std::weak_ptr<GameObject> go, std::string id, sf::Vector2f rSize, std::weak_ptr<sf::RenderWindow> renderWindow) : Component(go, id), size(rSize), window(renderWindow) {};
    ~CameraCP() = default;

    void init() override;
    void update(float deltaTime) override;

    std::string getComponentId()override { return this->componentId; }
    void setComponentId(std::string id) { this->componentId = id; }

    void rotate(float degree) { view.rotate(degree); }
    void zoom(float zoom) { view.rotate(zoom); }
    void setSize(sf::Vector2f size) { view.setSize(size); }
    void setViewPort(sf::FloatRect viewPort){ view.setViewport(viewPort); }

    std::shared_ptr<sf::RenderWindow> getWindow() { return window; }

protected:
    sf::Vector2f size;
    sf::View view;
    std::shared_ptr<TransformationCP> transform;
    std::shared_ptr<sf::RenderWindow> window;
};