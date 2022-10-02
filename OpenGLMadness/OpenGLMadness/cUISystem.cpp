#include "cUISystem.h"
#include "cGameEngine.h"
#include <iostream>

cUISystem::cUISystem()
{
}

cUISystem::~cUISystem()
{
}

void cUISystem::Initialize(cGameEngine* engine)
{
	this->engine = engine;
}

void cUISystem::Process(const std::vector<cEntity*>& entities, float dt)
{
}

void cUISystem::Process(cUIEntity* master, float dt, bool first)
{
	glDisable(GL_DEPTH_TEST);
	std::vector<cUIEntity*> children = master->GetChildren();
	for (std::vector<cUIEntity*>::const_iterator it = children.begin(); it != children.end(); it++)
	{
		cUIEntity* element = *it;

		comp::cMeshRenderer* renderer = element->GetComponent<comp::cMeshRenderer>();

		if (renderer == 0 || element->hide)
		{
			continue;
		}

		std::string oldShaderName = engine->shaderManager.GetCurrentShaderName();
		engine->shaderManager.UseShader("simpleUI");
		cShader* shader = engine->shaderManager.GetCurrentShader();

		shader->SetFloat("screenWidth", (float)engine->window_x);
		shader->SetFloat("screenHeight", (float)engine->window_y);

		shader->SetFloat("width", (float)element->width);
		shader->SetFloat("height", (float)element->height);

		//std::cout << element->x << ", " << element->y << "\n";

		shader->SetVec3("position", glm::vec3(element->x, element->y, 0));

		std::vector<sMeshData> meshes;
		engine->meshManager.GetMeshByName(renderer->meshName, meshes);

		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			glBindVertexArray(meshes[i].VAO_ID);
			glDrawElements(GL_TRIANGLES, meshes[i].numberOfIndices, GL_UNSIGNED_INT, 0);
		}

		this->engine->shaderManager.UseShader(oldShaderName);

		this->Process(element, dt, false);

	}
	glEnable(GL_DEPTH_TEST);
}
