#pragma once

#include "cComponent.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace comp
{
	class cCharacterController : public cComponent
	{
	public:
		btKinematicCharacterController* charCon;
		btPairCachingGhostObject* ghostObject;
	};
}
