#pragma once

#include "cComponent.h"
#include <btBulletCollisionCommon.h>

namespace comp
{
	class cPhysics : public cComponent
	{
	public:
		btRigidBody* rb;
	};
}