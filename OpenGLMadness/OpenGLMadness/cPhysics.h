#pragma once

#include "cComponent.h"
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace comp
{
	class cPhysics : public cComponent
	{
	public:
		btRigidBody* rb;
		btGhostObject* ghost;
	};
}