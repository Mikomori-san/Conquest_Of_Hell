//MultiMediaTechnology FHS
//MultiMediaProjekt 2a
//Kevin Raffetseder, Julian Resch, Jennifer Strohmer
#pragma once
#include "../Components/Render_Components/RenderCP.h"
#include <vector>

class RenderManager
{
public:
	static RenderManager& getInstance()
	{
		static RenderManager instance;
		return instance;
	}

	void render();
	void addToLayers(std::shared_ptr<RenderCP> renderCP);
	void resetLayers(std::vector<std::shared_ptr<RenderCP>> newLayersToRender);
	std::vector<std::shared_ptr<RenderCP>>& getLayers() { return layersToRender; }

private:
	RenderManager() = default;
	~RenderManager() = default;

	std::vector<std::shared_ptr<RenderCP>> layersToRender;
};