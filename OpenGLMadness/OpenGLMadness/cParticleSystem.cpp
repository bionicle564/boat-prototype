#include "cParticleSystem.h"
#include "cGameEngine.h"
#include "cParticleGenerator.h"
#include "cPosition.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

cParticleSystem::cParticleSystem()
{
	deltaTime = 0.f;
	engine = NULL;
}

cParticleSystem::~cParticleSystem()
{
}

void cParticleSystem::Initialize(cGameEngine* engine)
{
	this->engine = engine;
}

void cParticleSystem::SetDeltaTime(float dt)
{
	deltaTime = dt;
}

void cParticleSystem::Process(const std::vector<cEntity*>& entities, float dt)
{
	dt = deltaTime;
	for (std::vector<cEntity*>::const_iterator it = entities.begin(); it != entities.end(); it++)
	{
		cEntity* entity = *it;

		comp::cParticleGenerator* generator = entity->GetComponent<comp::cParticleGenerator>();
		
		if (generator == 0)
		{
			continue;
		}

		engine->shaderManager.UseShader(generator->shader);
		cShader* shader = engine->shaderManager.GetCurrentShader();
		
		//TODO: set this in the renderer at run time
		GLuint offsLoc = glGetProgramResourceIndex(shader->ID, GL_SHADER_STORAGE_BLOCK, "OffsetsBlock");
		if (offsLoc != GL_INVALID_INDEX)
		{
			glShaderStorageBlockBinding(shader->ID, offsLoc, generator->bindPoint);
		} 

		generator->Update(dt, glm::vec4(engine->cameraManager.GetMainCamera()->Position,0), glm::vec4(entity->GetComponent<comp::cPosition>()->position, 1));

		comp::cMeshRenderer* renderer = generator->particleMesh;

		if (renderer == 0)
		{
			continue;
		}

		glm::vec3 location(0);
		comp::cPosition* pos = 0;// entity->GetComponent<comp::cPosition>();
		if (pos != 0)
		{
			location = pos->position;
		}

		glm::quat orientation(1, 0, 0, 0);
		glm::vec3 size(1);




		std::vector<sMeshData> meshes;

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, location);
		model *= glm::toMat4(orientation);
		model = glm::scale(model, size);

		//this->engine->shaderManager.UseShader("name from the component");
		

		shader->SetMat4("model", model);

		engine->meshManager.GetMeshByName(renderer->meshName, meshes);

		if (renderer->billboard)
		{
			shader->SetBool("bBillboard", renderer->billboard);
		}
		else
		{
			shader->SetBool("bBillboard", renderer->billboard);
		}

		cFlyCamera* main = this->engine->cameraManager.GetMainCamera();

		shader->SetVec3("cameraRight", main->Right);
		shader->SetVec3("cameraUp", main->Up);

		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			glBindVertexArray(meshes[i].VAO_ID);
			//glDrawElements(GL_TRIANGLES, meshes[i].numberOfIndices, GL_UNSIGNED_INT, 0);
			glDrawElementsInstanced(GL_TRIANGLES, meshes[i].numberOfIndices, GL_UNSIGNED_INT, 0, generator->amount);
		}

	}
}
