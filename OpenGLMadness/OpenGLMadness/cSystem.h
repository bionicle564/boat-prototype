#pragma once

#include <vector>
#include "cEntity.h"

class cSystem
{
public:
	virtual void Process(const std::vector<cEntity*>& entities, float dt) = 0;
};