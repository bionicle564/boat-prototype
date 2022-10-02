#pragma once
#include "cSystem.h"
#include "cUIEntity.h"

class cGameEngine;

class cUISystem : cSystem
{
private:
	cGameEngine* engine;

	const float vertices[12] = {
		// positions       
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	const int elements[6] = {
		1,2,3,
		0,2,3
	};

public:
	cUISystem();
	virtual ~cUISystem();

	void Initialize(cGameEngine* engine);
	
	virtual void Process(const std::vector<cEntity*>& entities, float dt);
	void Process(cUIEntity* master, float dt, bool first);
};