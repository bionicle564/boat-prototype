#pragma once
#include "iBoat.h"

class cRaft : public iBoat
{
private:

	std::vector<cPart*> parts;


public:

	cRaft();
	virtual ~cRaft();

	virtual std::vector<cPart*>& GetBoatParts();

	virtual void Update(float deltaTime);
};