#include "cRenderSystem.h"
#include "cGameEngine.h"

#include "cPosition.h"
#include "cRotation.h"
#include "cScale.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

cRenderSystem::cRenderSystem()
{
	
}

cRenderSystem::~cRenderSystem()
{
}

void cRenderSystem::Initialize(cGameEngine* engine)
{
	this->engine = engine;
	
}

void cRenderSystem::Process(const std::vector<cEntity*>& entities, float dt)
{
	//glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glStencilMask(0x00);
	//draw things without border

	//glStencilFunc(GL_ALWAYS, 1, 0xff);
	//glStencilMask(0xff);
	//draw normally

	for (std::vector<cEntity*>::const_iterator it = entities.begin(); it != entities.end(); it++)
	{
		glStencilMask(0x00);
		cEntity* entity = *it;

		comp::cMeshRenderer* renderer = entity->GetComponent<comp::cMeshRenderer>();

		if (renderer == 0)
		{
			continue; 
		}

		glm::vec3 location(0);
		comp::cPosition* pos = entity->GetComponent<comp::cPosition>();
		if (pos != 0)
		{
			location = pos->position;
		}
		
		glm::quat orientation(1, 0, 0, 0);
		comp::cRotation* rot = entity->GetComponent<comp::cRotation>();
		if (rot != 0)
		{
			orientation = rot->rotation;
		}
		
		glm::vec3 size(1);
		comp::cScale* scale = entity->GetComponent<comp::cScale>();
		if (scale != 0)
		{
			size = scale->scale;
		}

		

		std::vector<sMeshData> meshes;

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, location);
		model *= glm::toMat4(orientation);
		model = glm::scale(model, size);

		this->engine->shaderManager.UseShader("default");
		cShader* shader = this->engine->shaderManager.GetCurrentShader();

		shader->SetMat4("model", model); 

		engine->meshManager.GetMeshByName(renderer->meshName, meshes);

		glActiveTexture(GL_TEXTURE0);
		GLuint diffuseTextureNumber = engine->textureManager.GetTexture(renderer->diffuseTexture);


		if (renderer->billboard)
		{
			shader->SetBool("bBillboard", renderer->billboard);

			shader->SetVec3("centerPos", pos->position);
			cFlyCamera* main = this->engine->cameraManager.GetMainCamera();

			shader->SetVec3("cameraRight", main->Right);
			shader->SetVec3("cameraUp", main->Up);


		}
		else
		{
			shader->SetBool("bBillboard", renderer->billboard);
		}

		if (!renderer->border)
		{
			
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				glBindVertexArray(meshes[i].VAO_ID);
				//glDrawElements(GL_TRIANGLES, meshes[i].numberOfIndices, GL_UNSIGNED_INT, 0);
			}
		}
		else //TODO: this needs to be fixed with the help of a fresh pair of eyes
		{
			glStencilFunc(GL_ALWAYS, 1, 0xff);
			glStencilMask(0xff);

			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				glBindVertexArray(meshes[i].VAO_ID);
				glDrawElements(GL_TRIANGLES, meshes[i].numberOfIndices, GL_UNSIGNED_INT, 0);
			}

			glStencilFunc(GL_NOTEQUAL, 1, 0xff);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			std::string beforeTimes = this->engine->shaderManager.GetCurrentShaderName();
			this->engine->shaderManager.UseShader("singleColour");

			glm::vec3 location(0);
			comp::cPosition* pos = entity->GetComponent<comp::cPosition>();
			if (pos != 0)
			{
				location = pos->position;
			}

			glm::quat orientation(1, 0, 0, 0);
			comp::cRotation* rot = entity->GetComponent<comp::cRotation>();
			if (rot != 0)
			{
				orientation = rot->rotation;
			}

			glm::vec3 size(1.03f);
			comp::cScale* scale = entity->GetComponent<comp::cScale>();
			if (scale != 0)
			{
				size = scale->scale;
			}

			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, location);
			model *= glm::toMat4(orientation);
			model = glm::scale(model, size);

			cShader* shader = this->engine->shaderManager.GetCurrentShader();

			shader->SetMat4("model", model);

			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				glBindVertexArray(meshes[i].VAO_ID);
				glDrawElements(GL_TRIANGLES, meshes[i].numberOfIndices, GL_UNSIGNED_INT, 0);
			}

			glStencilMask(0xff);
			glStencilFunc(GL_ALWAYS, 1, 0xff);
			glEnable(GL_DEPTH_TEST);

			this->engine->shaderManager.UseShader(beforeTimes);

		}

	}

	//glStencilFunc(GL_NOTEQUAL, 1, 0xff);
	//glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);
	//std::string beforeTimes = this->engine->shaderManager.GetCurrentShaderName();
	//this->engine->shaderManager.UseShader("singleColour");

	//for (std::vector<cEntity*>::const_iterator it = entities.begin(); it != entities.end(); it++)
	//{
	//	cEntity* entity = *it;

	//	comp::cMeshRenderer* renderer = entity->GetComponent<comp::cMeshRenderer>();

	//	if (renderer == 0)
	//	{
	//		continue;
	//	}

	//	glm::vec3 location(0);
	//	comp::cPosition* pos = entity->GetComponent<comp::cPosition>();
	//	if (pos != 0)
	//	{
	//		location = pos->position;
	//	}

	//	glm::quat orientation(1, 0, 0, 0);
	//	comp::cRotation* rot = entity->GetComponent<comp::cRotation>();
	//	if (rot != 0)
	//	{
	//		orientation = rot->rotation;
	//	}

	//	glm::vec3 size(1.03);
	//	comp::cScale* scale = entity->GetComponent<comp::cScale>();
	//	if (scale != 0)
	//	{
	//		size = scale->scale;
	//	}


	//	std::vector<sMeshData> meshes;

	//	glm::mat4 model = glm::mat4(1);
	//	model = glm::translate(model, location);
	//	model *= glm::toMat4(orientation);
	//	model = glm::scale(model, size);

	//	//this->engine->shaderManager.UseShader("name from the component");
	//	cShader* shader = this->engine->shaderManager.GetCurrentShader();

	//	shader->SetMat4("model", model);

	//	engine->meshManager.GetMeshByName(renderer->meshName, meshes);

	//	for (unsigned int i = 0; i < meshes.size(); i++)
	//	{
	//		glBindVertexArray(meshes[i].VAO_ID);
	//		glDrawElements(GL_TRIANGLES, meshes[i].numberOfIndices, GL_UNSIGNED_INT, 0);
	//	}
	//}

	//glStencilMask(0xff);
	//glStencilFunc(GL_ALWAYS, 1, 0xff);
	//glEnable(GL_DEPTH_TEST);

	//this->engine->shaderManager.UseShader(beforeTimes);
}
