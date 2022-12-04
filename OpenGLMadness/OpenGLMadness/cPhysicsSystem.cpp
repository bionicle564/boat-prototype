#include "cPhysicsSystem.h"
#include "cPosition.h"
#include "cPhysics.h"
#include "cCharacterController.h"
#include "cActionArea.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

cPhysicsSystem::cPhysicsSystem()
{
}

cPhysicsSystem::~cPhysicsSystem()
{
}

void cPhysicsSystem::Initialize()
{
}

void cPhysicsSystem::Process(const std::vector<cEntity*>& entities, float dt)
{
	for (std::vector<cEntity*>::const_iterator it = entities.begin(); it != entities.end(); it++)
	{
		cEntity* entity = *it;

		comp::cCharacterController* charCon = entity->GetComponent<comp::cCharacterController>();
		if (charCon != 0)
		{

			btTransform transform;

			transform = charCon->ghostObject->getWorldTransform();

			comp::cPosition* pos = entity->GetComponent<comp::cPosition>();

			btVector3 btPos = transform.getOrigin();

			pos->position.x = btPos.x();
			pos->position.y = btPos.y();
			pos->position.z = btPos.z();
		}


		comp::cPhysics* phys = entity->GetComponent<comp::cPhysics>();
		if (phys == 0)
		{
			continue;
		}


		comp::cPosition* pos = entity->GetComponent<comp::cPosition>();
		if (pos == 0)
		{
			continue;
			//this is bad
		}

		comp::cActionArea* area = entity->GetComponent<comp::cActionArea>();
		if (area != 0)
		{
			area->area->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos->position.x, pos->position.y, pos->position.z)));
		}


		btRigidBody* body = phys->rb;
		
		btTransform rbPos;
		btVector3 po = body->getWorldTransform().getOrigin();
		body->getMotionState()->getWorldTransform(rbPos);
		
		pos->position.x = po.x();
		pos->position.y = po.y();
		pos->position.z = po.z();

	}
}
