#include "cPhysicsSystem.h"
#include "cPosition.h"
#include "cPhysics.h"
#include <btBulletCollisionCommon.h>

cPhysicsSystem::cPhysicsSystem()
{
}

cPhysicsSystem::~cPhysicsSystem()
{
}

void cPhysicsSystem::Process(const std::vector<cEntity*>& entities, float dt)
{
	for (std::vector<cEntity*>::const_iterator it = entities.begin(); it != entities.end(); it++)
	{
		cEntity* entity = *it;

		comp::cPhysics* phys = entity->GetComponent<comp::cPhysics>();
		if (phys == 0)
		{
			continue;
		}

		comp::cPosition* pos = entity->GetComponent<comp::cPosition>();
		if (pos == 0)
		{
			//this is bad
		}

		btRigidBody* body = phys->rb;
		
		
		
	}
}
