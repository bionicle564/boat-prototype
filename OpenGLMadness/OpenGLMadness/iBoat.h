#pragma once
#include "cEntity.h"

class iBoat : public cEntity
{
public:

	iBoat();
	virtual ~iBoat();

	virtual void GetBoatParts() = 0;

};