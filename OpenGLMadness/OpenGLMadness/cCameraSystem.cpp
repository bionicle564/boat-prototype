#include "cCameraSystem.h"
#include "cCameraComp.h"
#include "GLCommon.h"
#include "cGameEngine.h"
#include "cCamera.h"

cCameraSystem::cCameraSystem()
{
}

cCameraSystem::~cCameraSystem()
{
}

void cCameraSystem::Initialize(cGameEngine* engine)
{
	this->engine = engine;
}

void cCameraSystem::Process(const std::vector<cEntity*>& entities, float dt)
{
	for (std::vector<cEntity*>::const_iterator it = entities.begin(); it != entities.end(); it++)
	{
		cEntity* entity = *it;

		comp::cCamera* camera = entity->GetComponent<comp::cCamera>();

		if (camera == 0)
		{
			continue;
		}

		if (camera->cameraId == 0) //main camera, no other cameras implemented yet
		{
			cFlyCamera* main = this->engine->cameraManager.GetMainCamera();

			cShader* shader = this->engine->shaderManager.GetCurrentShader();

			shader->SetVec3("cameraRight", main->Right);
			shader->SetVec3("cameraUp", main->Up);

			glm::mat4 view;
			view = main->GetViewMatrix();


			glBindBuffer(GL_UNIFORM_BUFFER, this->engine->shaderManager.uboMatId);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			this->engine->physicsManager.debugDrawerer.SetView(view);
			//shader->SetMat4("view", view);

			//std::string name = this->engine->shaderManager.GetCurrentShaderName();
			//this->engine->shaderManager.UseShader("singleColour");
			//this->engine->shaderManager.GetCurrentShader()->SetMat4("view", view);


			//this->engine->shaderManager.UseShader(name);

			return; //only do the one camera for now
		}

	}

}
