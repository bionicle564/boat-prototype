#pragma once

#include "cComponent.h"
#include <glm/vec3.hpp>

namespace comp
{
	//attaches a camera id to an entity, can be used for secondary rendering later
	class cCamera : public cComponent
	{
	public:
		unsigned int cameraId; //set id to 0 for the main camera
		
		glm::vec3 position;
		glm::vec3 lookAt;

		bool primaryCamera;

	};
}