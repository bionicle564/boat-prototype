#pragma once
#include "cPart.h"
#include <vector>

class iBoat //: public cEntity
{
public:
	cEntity* ent;

	iBoat() { }
	virtual ~iBoat() {}

	virtual std::vector<cPart*>& GetBoatParts() = 0;

	virtual void Update(float deltaTime) = 0;
};