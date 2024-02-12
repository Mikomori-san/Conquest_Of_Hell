//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "Components/Component.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>


class Component;

class GameObject
{
public:
	GameObject(std::string id) : m_id(id){}

	~GameObject(){}

	void addComponent(std::shared_ptr<Component> component);
	void update(float deltaTime);
    void init();
	std::vector<std::shared_ptr<Component>>& getComponents() { return m_components; }
	
	template<typename T = Component>
	std::vector<std::shared_ptr<T>> getComponentsOfType();

	std::shared_ptr<Component> getComponent(std::string id);
	std::string getId() { return  this->m_id; }
	void setId(std::string id) { this->m_id = id; }

private:

	std::string m_id;
	std::vector<std::shared_ptr<Component>> m_components;
};

template<typename T>
inline std::vector<std::shared_ptr<T>> GameObject::getComponentsOfType()
{
	std::vector<std::shared_ptr<T>> comps;
	std::shared_ptr<T> specComp;
	for (auto& comp : m_components)
	{
		if (specComp = std::dynamic_pointer_cast<T>(comp))
		{
			comps.push_back(specComp);
		}
	}
	return comps;
}
