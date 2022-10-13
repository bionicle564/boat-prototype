#include "cGame.h"
#include "GLCommon.h"
#include "cMeshRenderer.h"
#include "cCameraComp.h"
#include "cPosition.h"
#include "cRotation.h"
#include "cScale.h"
#include "cParticleGenerator.h"
#include "cPhysicsManager.h"
#include <iostream>
#include "cPlayer.h"

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
	camera->AddComponent<comp::cCamera>()->cameraId = 1;
	camera->GetComponent<comp::cCamera>()->primaryCamera = true;


	ent = engine.entityManager.CreateEntity();
	ent->AddComponent<comp::cMeshRenderer>()->meshName = "billboard.fbx";
	ent->GetComponent<comp::cMeshRenderer>()->billboard = false;
	ent->AddComponent<comp::cPosition>()->position = glm::vec3(1);
	ent->AddComponent<comp::cRotation>()->rotation = glm::quat(glm::vec3(0, 0, 0));
	//ent->AddComponent<comp::cScale>()->scale = glm::vec3(1);

	sBodyDesc desc;
	desc.halfExtents = glm::vec4(6,1,6,0);
	desc.mass = 1;
	desc.position = glm::vec3(0, -3, -10);
	desc.type = eBodyType::BOX;
	desc.orientation = glm::quat(glm::vec3(0,0,0));
	desc.friction = 1;
	desc.kinematic = true;

	ent->AddComponent(engine.physicsManager.MakeBody(desc));
	
	
	box = engine.entityManager.CreateEntity();
	box->AddComponent<comp::cPosition>()->position = glm::vec3(1);

	desc.halfExtents = glm::vec4(.3);
	desc.mass = 100;
	desc.position = glm::vec3(2, 0, -10);
	desc.type = eBodyType::BOX;
	desc.kinematic = true;
	box->AddComponent(engine.physicsManager.MakeBody(desc));
	desc.kinematic = false;


	dude = engine.entityManager.CreateEntity();

	dude->AddComponent<comp::cMeshRenderer>()->meshName = "fixed_knight.fbx";
	dude->GetComponent<comp::cMeshRenderer>()->border = false;
	dude->AddComponent<comp::cScale>()->scale = glm::vec3(1);
	dude->AddComponent<comp::cPosition>();
	//dude->AddComponent<comp::cParticleGenerator>(); //something is scuffed here

	
	desc.halfExtents = glm::vec4(.75,2,0,0);
	desc.mass = 1;
	desc.position = glm::vec3(0, 3, -10);
	desc.type = eBodyType::CAPSULE;
	desc.orientation = glm::quat(glm::vec3(0));
	//desc.orientation = glm::quat(glm::vec3(glm::half_pi<float>(),0,0));
	desc.friction = 1;

	dude->AddComponent(engine.physicsManager.MakeBody(desc));
	btRigidBody* rb = dude->GetComponent<comp::cPhysics>()->rb;
	rb->setAngularFactor(btVector3(0, 0, 0));
	//dude->AddComponent(engine.physicsManager.MakeController(desc));

	cPlayer* player = (cPlayer*)engine.entityManager.CreateEntity();
	player->SetUp(engine);

	int uasdf = 9;
}

void cGame::Update()
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//update camera position based on player position
	camera->GetComponent<comp::cCamera>()->position = dude->GetComponent<comp::cPosition>()->position + glm::vec3(0, 18, 3);
	camera->GetComponent<comp::cCamera>()->lookAt = dude->GetComponent<comp::cPosition>()->position;

	


	btRigidBody* rb = ent->GetComponent<comp::cPhysics>()->rb;
	
	//simple boat movment
	btTransform newTrans;
	rb->getMotionState()->getWorldTransform(newTrans);
	newTrans.getOrigin() += (btVector3(.5f, 0, 0) * deltaTime);
	rb->getMotionState()->setWorldTransform(newTrans);



	glfwGetWindowSize(window, &winX, &winY);

	Input(deltaTime);

	engine.Update(deltaTime, winX, winY);


	engine.Render();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void cGame::Input(float dt)
{
	btRigidBody* rb = dude->GetComponent<comp::cPhysics>()->rb;

	bool j = false;
	//bool j = dude->GetComponent<comp::cCharacterController>()->charCon->onGround();
	//std::cout << j << "\n";
	btVector3 dir(0,0,0);

	comp::cCharacterController* ph = NULL;// dude->GetComponent<comp::cCharacterController>();
	if (engine.m_KeyDown[' '])
	{
		if (j)
		{
			ph->charCon->jump(btVector3(0, 4, 0));
		}
	}

	btVector3 boatSpeed = ent->GetComponent<comp::cPhysics>()->rb->getLinearVelocity();
	btVector3 speed = rb->getLinearVelocity();

	std::cout << speed.x() << ", " << boatSpeed.x() << "\n";

	//right
	if (engine.m_KeyDown['L'])
	{
		dir.setX(3);
	}

	//left
	if (engine.m_KeyDown['J'])
	{
		dir.setX(-3);
	}

	//up
	if (engine.m_KeyDown['I'])
	{
		dir.setZ(-3);
	}

	//down
	if (engine.m_KeyDown['K'])
	{
		dir.setZ(3);
	}

	if (dir.length() > 1)
	{
		//rb->clearGravity();

		rb->setLinearVelocity(dir + boatSpeed);

		//rb->applyCentralForce(dir);
	}
	else
	{
		speed.setX(boatSpeed.x());
		speed.setZ(boatSpeed.z());

		rb->setLinearVelocity(speed);
		//rb->setLinearVelocity(boatSpeed);
	}
}
