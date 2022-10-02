#pragma once

#include "cComponent.h"

namespace comp
{
	//attaches a camera id to an entity, can be used for secondary rendering later
	class cCamera : public cComponent
	{
	public:
		unsigned int cameraId; //set id to 0 for the main camera
	};
}