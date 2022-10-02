#pragma once
#include "cComponent.h"
#include <glm/vec3.hpp>

namespace comp
{
	class cScale : public cComponent
	{
	public:
		cScale() 
		{
			scale = glm::vec3(1);
		}

		glm::vec3 scale;

	};
}