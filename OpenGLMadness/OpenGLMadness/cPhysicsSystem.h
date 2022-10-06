#pragma once
#include "cSystem.h"

class cPhysicsSystem : cSystem
{
public:
	cPhysicsSystem();
	virtual ~cPhysicsSystem();

	void Initialize();
	virtual void Process(const std::vector<cEntity*>& entities, float dt);
};