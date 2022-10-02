#pragma once
#include "cComponent.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

namespace comp
{
	class cRotation :public cComponent
	{
	public:
		cRotation() 
		{
			rotation = glm::quat(1,0,0,0);
		}

		void Rotate(glm::vec3 axis, float angle)
		{
			glm::quat newRot;

			newRot.x = axis.x * sin(angle / 2);
			newRot.y = axis.y * sin(angle / 2);
			newRot.z = axis.z * sin(angle / 2);
			newRot.w = cos(angle / 2);
			
			//rotation = newRot * rotation;
			rotation *= newRot;

		}

		glm::quat rotation;
	};
}