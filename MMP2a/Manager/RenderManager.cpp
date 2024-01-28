#include "stdafx.h"
#include "RenderManager.h"
#include <algorithm>
#include "InputManager.h"

bool compareByLayerNr(const std::shared_ptr<RenderCP>& a, const std::shared_ptr<RenderCP>& b);

void RenderManager::render()
{
	std::sort(layersToRender.begin(), layersToRender.end(), compareByLayerNr);

	for (auto& comp : layersToRender)
	{
		comp->draw();
	}
}

void RenderManager::addToLayers(std::shared_ptr<RenderCP> renderCP)
{
	layersToRender.push_back(renderCP);
}

bool compareByLayerNr(const std::shared_ptr<RenderCP>& a, const std::shared_ptr<RenderCP>& b) {
	return a->getLayerNr() < b->getLayerNr();
}

