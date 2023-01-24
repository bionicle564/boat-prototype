#include "cGameEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

cGameEngine::cGameEngine(GLFWwindow* window)
{
	this->mainWindowRef = window;
}

cGameEngine::cGameEngine()
{
	this->mainWindowRef = NULL;
}

cGameEngine::~cGameEngine()
{
	this->entityManager.ShutDown();
	this->meshManager.ShutDown();
	this->shaderManager.ShutDown();
	this->cameraManager.ShutDown();
	this->physicsManager.ShutDown();
}

void cGameEngine::SetWindow(GLFWwindow* window)
{
	this->mainWindowRef = window;
}

bool cGameEngine::Initialize()
{
	this->entityManager.StartUp();
	this->meshManager.StartUp();
	this->shaderManager.StartUp();
	this->cameraManager.StartUp();
	this->textureManager.StartUp();
	this->physicsManager.StartUp();

	this->physicsManager.debugDrawerer.program = this->shaderManager.GetIDFromName("debugPhysics");

	this->renderer.Initialize(this);
	this->cameraHandler.Initialize(this);
	this->uiSystem.Initialize(this);
	this->particleSystem.Initialize(this);
	this->physicsSystem.Initialize();

	return true;
}

void cGameEngine::Destroy()
{
}

void cGameEngine::Update(float dt, int winX, int winY)
{
	this->window_x = winX;
	this->window_y = winY;

	UpdateInput(dt);

	this->particleSystem.SetDeltaTime(dt);
 	this->cameraHandler.Process(this->entityManager.GetEntities(), dt);

	this->physicsManager.UpdatePhysics(dt);

	this->physicsSystem.Process(this->entityManager.GetEntities(), dt);

}

void cGameEngine::Render()
{

	cFlyCamera* mainCamera = this->cameraManager.GetMainCamera();

	glm::mat4 projection = glm::perspective(glm::radians(mainCamera->Zoom), (float)window_x / (float)window_y, 0.1f, 1000.0f);

	//update the part of the buffer with the projection
	glBindBuffer(GL_UNIFORM_BUFFER, shaderManager.uboMatId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	

	//this->physicsManager.debugDrawerer.SetMatrices();

	this->renderer.Process(this->entityManager.GetEntities(), 0);

 	this->physicsManager.DebugDraw();

	this->particleSystem.Process(this->entityManager.GetEntities(), 0);

	this->uiSystem.Process(this->entityManager.masterCanvas, 0, true);

}

void cGameEngine::PressKey(unsigned char key)
{
	m_KeyDown[key] = true;
}

void cGameEngine::ReleaseKey(unsigned char key)
{
	m_KeyDown[key] = false;
}

void cGameEngine::ProcessMouse(double xpos, double ypos)
{
	float xpos2 = static_cast<float>(xpos);
	float ypos2 = static_cast<float>(ypos);

	if (firstMouse)
	{
 		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top
	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.8f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (!selectMode)
	{
		cameraManager.GetMainCamera()->ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		lastSelectX = (float)xpos;
		lastSelectY = (float)ypos;

		this->entityManager.cursor->x = lastSelectX - 10.f;
		this->entityManager.cursor->y = -lastSelectY + 10.f;

		if (this->entityManager.cursor->x < 0) //clamp the cursor to the left edge, doesn't work perfectly
		{
			this->entityManager.cursor->x = 0;
			lastSelectX = 1;
		}
		else
		{
			lastSelectX = (float)xpos;
		}
		
		//std::cout << lastSelectX << "\n";

	}
	

}

void cGameEngine::ToggleSelectMode()
{
	this->selectMode = !this->selectMode;
	this->entityManager.cursor->hide = !selectMode;
	if (selectMode)
	{
		glfwSetCursorPos(this->mainWindowRef, lastSelectX, lastSelectY);
		this->entityManager.cursor->x = lastSelectX - 10.f;
		this->entityManager.cursor->y = -lastSelectY + 10.f;
		
	}
}

bool cGameEngine::GetSelectMode()
{
	return selectMode;
}

void cGameEngine::UpdateInput(float dt)
{

	cFlyCamera* mainCamera = this->cameraManager.GetMainCamera();
	float speed = 10;
	if (m_KeyDown['W'])
	{
		mainCamera->ProcessKeyboard(Camera_Movement::FORWARD, dt * speed);
	}

	if (m_KeyDown['A'])
	{
		mainCamera->ProcessKeyboard(Camera_Movement::LEFT, dt* speed);
	}

	if (m_KeyDown['S'])
	{
		mainCamera->ProcessKeyboard(Camera_Movement::BACKWARD, dt* speed);
	}

	if (m_KeyDown['D'])
	{
		mainCamera->ProcessKeyboard(Camera_Movement::RIGHT, dt* speed);
	}
}
