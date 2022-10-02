#include "cEntity.h"

cEntity::cEntity()
{
}

cEntity::~cEntity()
{
	for (std::vector<cComponent*>::iterator it = components.begin(); it != components.end(); it++)
	{
		delete* it;
	}
}

void cEntity::RemoveComponent(cComponent* component)
{
}
