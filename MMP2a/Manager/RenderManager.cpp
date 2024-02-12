//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#include "stdafx.h"
#include "InputManager.h"
#include "RenderManager.h"
#include <algorithm>
#include <iostream>

bool compareByLayerNr(const std::shared_ptr<RenderCP>& a, const std::shared_ptr<RenderCP>& b);

void RenderManager::render()
{
	std::sort(m_layersToRender.begin(), m_layersToRender.end(), compareByLayerNr);

	for (auto& comp : m_layersToRender)
	{
		comp->draw();
	}
}

void RenderManager::addToLayers(std::shared_ptr<RenderCP> renderCP)
{
	m_layersToRender.push_back(renderCP);
}

bool compareByLayerNr(const std::shared_ptr<RenderCP>& a, const std::shared_ptr<RenderCP>& b) {
	return a->getLayerNr() < b->getLayerNr();
}

void RenderManager::resetLayers(std::vector<std::shared_ptr<RenderCP>> newLayersToRender) 
{
	m_layersToRender = newLayersToRender;
}