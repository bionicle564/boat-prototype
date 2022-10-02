#pragma once
#include "cComponent.h"
#include "GLCommon.h"
#include <string>

namespace comp
{
	/// <summary>
	/// Attaches a mesh with a spacific shader to an entity.
	/// Will use the "default" shader by default
	/// </summary>
	class cMeshRenderer : public cComponent
	{
	public:
		unsigned int meshID=0;
		std::string meshName; //set this to the model you want to use

		std::string shader = "default"; //TODO: implement this

		std::string diffuseTexture = "default";

		bool border = false;
		bool billboard = false;
	};
}