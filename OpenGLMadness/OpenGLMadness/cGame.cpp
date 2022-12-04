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
#include "cGameObject.h"

#include <BulletCollision/CollisionShapes/btTriangleCallback.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

cGame::cGame()
{
	done = false;
	camera = NULL;
}

cGame::~cGame()
{
	delete boat;
	delete player;
	delete box;
}

void cGame::Init(GLFWwindow* window)
{
	this->window = window;
	engine.SetWindow(window);
	engine.Initialize();

	cGameObject::SetFactory(&engine.entityManager);

	engine.meshManager.LoadMesh("box.fbx");
	engine.meshManager.LoadMesh("capsule.fbx");
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


	boat = new cRaft();




	boat->AddComponent<comp::cMeshRenderer>()->meshName = "box.fbx";
	boat->AddComponent<comp::cPosition>()->position = glm::vec3(1);
	boat->AddComponent<comp::cRotation>()->rotation = glm::quat(glm::vec3(0, 0, 0));
	boat->AddComponent<comp::cScale>()->scale = glm::vec3(11.5, 1.9, 11.5);

	sBodyDesc desc;
	desc.halfExtents = glm::vec4(6, 1, 6, 0);
	desc.mass = 1;
	desc.position = glm::vec3(0, -3, -10);
	desc.type = eBodyType::BOX;
	desc.orientation = glm::quat(glm::vec3(0, 0, 0));
	desc.friction = 1;
	desc.kinematic = true;

	boat->AddComponent(engine.physicsManager.MakeBody(desc));

	desc.halfExtents = glm::vec4(1);
	//box
	box = new cGameObject();
	box->AddComponent<comp::cPosition>()->position = glm::vec3(1);
	box->AddComponent<comp::cActionArea>(engine.physicsManager.MakeActionArea(desc));

	desc.halfExtents = glm::vec4(.3);
	desc.mass = 1;
	desc.position = glm::vec3(2, 0, -10);
	desc.type = eBodyType::BOX;
	desc.kinematic = true;
	desc.friction = 1;
	box->AddComponent(engine.physicsManager.MakeBody(desc));
	desc.kinematic = false;

	//should need this here, boat class should handle that
	//this->engine.physicsManager.LinkObjectsPositions(boat->GetComponent<comp::cPhysics>(), box->GetComponent <comp::cPhysics>());

	//dude->AddComponent(engine.physicsManager.MakeController(desc));

	player = new cPlayer();
	player->SetUp(engine);

}

void cGame::Update()
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//update camera position based on player position
	camera->GetComponent<comp::cCamera>()->position = player->GetComponent<comp::cPosition>()->position + glm::vec3(0, 18, 3);
	camera->GetComponent<comp::cCamera>()->lookAt = player->GetComponent<comp::cPosition>()->position;
	
	boat->Update(deltaTime);


	glfwGetWindowSize(window, &winX, &winY);

	Input(deltaTime);

	engine.Update(deltaTime, winX, winY);


	engine.Render();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void cGame::Input(float dt)
{
	btRigidBody* rb = player->bodySelfRef;

	bool j = false;
	//bool j = dude->GetComponent<comp::cCharacterController>()->charCon->onGround();
	
	btVector3 dir(0,0,0);

	//raycast
	{
		btVector3 from = player->bodySelfRef->getWorldTransform().getOrigin();
		btVector3 to = from + btVector3(0,-2,0);
		

		btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
		//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

		engine.physicsManager.dynamicWorld->rayTest(from, to, closestResults);

		if (closestResults.hasHit())
		{
			j = true;
			btVector3 p = from.lerp(to, closestResults.m_closestHitFraction);
			//std::cout << "on ground\n";
			btTransform trans = player->bodySelfRef->getWorldTransform();
			btVector3 pos = trans.getOrigin();
			pos.setY(boat->GetComponent<comp::cPosition>()->position.y + 2.5);
			trans.setOrigin(pos);
			player->bodySelfRef->setWorldTransform(trans);
			
		}
	}

	//std::cout << j << "\n";

	if (engine.m_KeyDown[' '])
	{
		if (j)
		{
			player->bodySelfRef->applyCentralImpulse(btVector3(0, 9, 0) * dt);
		}
	}

	btVector3 boatSpeed = boat->GetComponent<comp::cPhysics>()->rb->getLinearVelocity();
	btVector3 speed = rb->getLinearVelocity();

	//std::cout << speed.x() << ", " << boatSpeed.x() << "\n";

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

		player->SetSpeed(dir + boatSpeed);

		//rb->applyCentralForce(dir);
	}
	else
	{
		speed.setX(boatSpeed.x());
		speed.setZ(boatSpeed.z());
		

		player->SetSpeed(speed);
		//rb->setLinearVelocity(boatSpeed);
	}
}
