#pragma once
#include "cComponent.h"
#include <glm/vec3.hpp>

namespace comp
{
	class cPosition : public cComponent
	{
	public:
		cPosition() 
		{
			position = glm::vec3(0);
		}

		glm::vec3 position;
	};
}