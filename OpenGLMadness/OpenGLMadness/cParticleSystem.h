#pragma once
#include "cSystem.h"

class cGameEngine;

class cParticleSystem : cSystem
{
private:
	cGameEngine* engine;
	float deltaTime;
public:
	cParticleSystem();
	virtual ~cParticleSystem();

	void Initialize(cGameEngine* engine);

	void SetDeltaTime(float dt);

	virtual void Process(const std::vector<cEntity*>& entities, float dt);
};