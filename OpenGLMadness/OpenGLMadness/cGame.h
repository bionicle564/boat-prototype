#pragma once
#include "cGameEngine.h"
#include "cEntity.h"

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

	cEntity* dude;

	cGameEngine engine;

	cGame();
	~cGame();

	void Init(GLFWwindow* window);

	//include render and process
	void Update();

	void Input(float dt);
};