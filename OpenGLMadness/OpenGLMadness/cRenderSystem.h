#pragma once
#include "cMeshRenderer.h"
#include "cSystem.h"

class cGameEngine;

class cRenderSystem : cSystem
{
private:
	cGameEngine* engine;

public:
	cRenderSystem();
	virtual ~cRenderSystem();

	void Initialize(cGameEngine* engine);

	virtual void Process(const std::vector<cEntity*>& entities, float dt);
};
