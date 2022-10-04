#include "cGame.h"
#include "GLCommon.h"
#include "cMeshRenderer.h"
#include "cCameraComp.h"
#include "cPosition.h"
#include "cRotation.h"
#include "cScale.h"
#include "cParticleGenerator.h"
#include "cPhysicsManager.h"

cGame::cGame()
{
	done = false;
	camera = NULL;
}

cGame::~cGame()
{
}

void cGame::Init(GLFWwindow* window)
{
	this->window = window;
	engine.SetWindow(window);
	engine.Initialize();

	engine.meshManager.LoadMesh("cylinder.fbx");
	engine.meshManager.LoadMesh("fixed_knight.fbx");
	engine.meshManager.LoadMesh("center_knight.fbx");
	engine.meshManager.LoadMesh("base_ui.fbx");
	engine.meshManager.LoadMesh("billboard.fbx");

	//set the samplers in the default shader
	engine.shaderManager.GetCurrentShader()->SetInt("texture_00", 0);
	engine.shaderManager.GetCurrentShader()->SetInt("texture_01", 1);


	camera = engine.entityManager.CreateEntity();
	camera->AddComponent<comp::cCamera>()->cameraId = 0;


	cEntity* ent = engine.entityManager.CreateEntity();
	ent->AddComponent<comp::cMeshRenderer>()->meshName = "billboard.fbx";
	ent->GetComponent<comp::cMeshRenderer>()->billboard = false;
	ent->AddComponent<comp::cPosition>()->position = glm::vec3(1);
	ent->AddComponent<comp::cRotation>()->rotation = glm::quat(glm::vec3(0, 0, 0));
	//ent->AddComponent<comp::cScale>()->scale = glm::vec3(1);


	cEntity* dude = engine.entityManager.CreateEntity();

	dude->AddComponent<comp::cMeshRenderer>()->meshName = "fixed_knight.fbx";
	dude->GetComponent<comp::cMeshRenderer>()->border = false;
	dude->AddComponent<comp::cScale>()->scale = glm::vec3(1);
	dude->AddComponent<comp::cPosition>();
	//dude->AddComponent<comp::cParticleGenerator>(); //something is scuffed here

	sBodyDesc desc;
	desc.halfExtents = glm::vec4(1);
	desc.mass = 0;
	desc.position = glm::vec3(-3, 0, 0);
	desc.type = eBodyType::BOX;

	dude->AddComponent(engine.physicsManager.MakeBody(desc));

}

void cGame::Update()
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glfwGetWindowSize(window, &winX, &winY);

	engine.Update(deltaTime, winX, winY);

	engine.Render();

	glfwSwapBuffers(window);
	glfwPollEvents();
}
