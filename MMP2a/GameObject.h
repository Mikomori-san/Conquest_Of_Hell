#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Components/Component.h"

class Component;

class GameObject
{
public:
	GameObject(std::string id) : id(id){}

	~GameObject(){}

	void addComponent(std::shared_ptr<Component> component);
	void update(float deltaTime);
    void init();
	std::vector<std::shared_ptr<Component>>& getComponents() { return components; }
	
	template<typename T = Component>
	std::vector<std::shared_ptr<T>> getComponentsOfType();

	std::shared_ptr<Component> getComponent(std::string id);
	std::string getId() { return  this->id; }
	void setId(std::string id) { this->id = id; }

private:

	std::string id;
	std::vector<std::shared_ptr<Component>> components;
};

template<typename T>
inline std::vector<std::shared_ptr<T>> GameObject::getComponentsOfType()
{
	std::vector<std::shared_ptr<T>> comps;
	std::shared_ptr<T> specComp;
	for (auto& comp : components)
	{
		if (specComp = std::dynamic_pointer_cast<T>(comp))
		{
			comps.push_back(specComp);
		}
	}
	return comps;
}
