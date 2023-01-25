#pragma once
#include "cMeshManager.h"
#include "cEntityManager.h"
#include "cRenderSystem.h"
#include "cCameraManager.h"
#include "cShaderManager.h"
#include "cCameraSystem.h"
#include "cUISystem.h"
#include "cParticleSystem.h"
#include "cTextureManager.h"
#include "cPhysicsManager.h"
#include "cPhysicsSystem.h"

#include "GLCommon.h"

//TODO: Comment how to use the code (clean your code you dolt)

class cGameEngine
{
private:
	cRenderSystem renderer;
	cCameraSystem cameraHandler;
	cUISystem uiSystem;
	cParticleSystem particleSystem;
	cPhysicsSystem physicsSystem;

	GLFWwindow* mainWindowRef;

	

	bool selectMode = false;


	//camera variables
	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 1280.0f / 2.0;
	float lastY = 700.0 / 2.0;
	float fov = 45.0f;

	float lastSelectX;
	float lastSelectY;

	


	void UpdateInput(float dt);

public:
	cGameEngine(GLFWwindow* window);
	cGameEngine();
	~cGameEngine();

	void SetWindow(GLFWwindow* window);

	bool Initialize();
	void Destroy();

	//update the game state and window information
	void Update(float dt, int winX, int winY);

	//render all renderalble entities
	void Render();

	//tell the engine a key is pressed
	void PressKey(unsigned char key);

	//tell the engine a key is released
	void ReleaseKey(unsigned char key);

	//handle the mouse position
	void ProcessMouse(double xpos, double ypos);

	void ToggleSelectMode();
	bool GetSelectMode();

	cMeshManager meshManager;
	cEntityManager entityManager;
	cCameraManager cameraManager;
	cShaderManager shaderManager;
	cTextureManager textureManager;
	cPhysicsManager physicsManager;


	int window_x;
	int window_y;

	bool m_KeyDown[256];

	bool debugRender;
};