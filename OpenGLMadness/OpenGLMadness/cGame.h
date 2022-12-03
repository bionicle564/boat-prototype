#pragma once
#include "cGameEngine.h"
#include "cEntity.h"
#include "cPlayer.h"
#include "cRaft.h"
#include "iBoat.h"

class cGame
{
private:
	GLFWwindow* window;
	cEntity* camera;


	int winX, winY;

	bool done;
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	float bx;
	float by;

public:

	cGameObject* box;

	cPlayer* player;

	iBoat* boat;

	cGameEngine engine;

	cGame();
	~cGame();

	void Init(GLFWwindow* window);

	//include render and process
	void Update();

	void Input(float dt);
};