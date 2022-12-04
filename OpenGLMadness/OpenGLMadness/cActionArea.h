#pragma once

#include "cComponent.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace comp
{
	//attaches a camera id to an entity, can be used for secondary rendering later
	class cActionArea : public cComponent
	{
	public:

		cActionArea() = default;
		virtual ~cActionArea() = default;

		btGhostObject* area;


	};
}