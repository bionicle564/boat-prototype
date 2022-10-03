#pragma once
#include "cSystem.h"

class cPhysicsSystem : cSystem
{
public:
	cPhysicsSystem();
	virtual ~cPhysicsSystem();

	virtual void Process(const std::vector<cEntity*>& entities, float dt);
};