#include "cGameObject.h"

cEntityManager* cGameObject::factory = NULL;

void cGameObject::SetFactory(cEntityManager* fac)
{
	cGameObject::factory = fac;
}

cGameObject::cGameObject()
{
	ent = NULL;

	if (cGameObject::factory != NULL)
	{
		ent = cGameObject::factory->CreateEntity();
	}
}