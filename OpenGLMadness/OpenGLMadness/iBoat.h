#pragma once
#include "cPart.h"
#include <vector>
#include "cGameObject.h"

class iBoat : public cGameObject
{
public:
	

	iBoat() 
	{
	}

	virtual ~iBoat() {}

	virtual std::vector<cPart*>& GetBoatParts() = 0;

	virtual void Update(float deltaTime) = 0;
};