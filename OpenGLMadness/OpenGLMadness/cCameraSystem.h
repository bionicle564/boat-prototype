#pragma once
#include "cSystem.h"


class cGameEngine;

class cCameraSystem : cSystem
{
private:
	cGameEngine* engine;
public:
	cCameraSystem();
	virtual ~cCameraSystem();

	void Initialize(cGameEngine* engine);

	virtual void Process(const std::vector<cEntity*>& entities, float dt);
};