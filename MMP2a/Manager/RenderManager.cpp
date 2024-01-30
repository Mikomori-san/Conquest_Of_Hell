#include "stdafx.h"
#include "RenderManager.h"
#include <algorithm>
#include "InputManager.h"

bool compareByLayerNr(const std::shared_ptr<RenderCP>& a, const std::shared_ptr<RenderCP>& b);

void RenderManager::render()
{
	std::sort(layersToRender.begin(), layersToRender.end(), compareByLayerNr);

	std::vector<std::shared_ptr<RenderCP>> newLayersToRender;

	for (auto& comp : layersToRender)
	{
		if (!comp->getGameObjectRef().expired())
		{
			comp->draw();
			newLayersToRender.push_back(comp);
		}
	}
	layersToRender = newLayersToRender;
}

void RenderManager::addToLayers(std::shared_ptr<RenderCP> renderCP)
{
	layersToRender.push_back(renderCP);
}

bool compareByLayerNr(const std::shared_ptr<RenderCP>& a, const std::shared_ptr<RenderCP>& b) {
	return a->getLayerNr() < b->getLayerNr();
}

