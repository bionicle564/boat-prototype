#pragma once
#include <vector>
#include "cCamera.h"

class cCameraManager
{
private:
	cFlyCamera* mainCamera;

public:
	cCameraManager();
	~cCameraManager();

	void StartUp();
	void ShutDown();

	cFlyCamera* GetMainCamera();

};