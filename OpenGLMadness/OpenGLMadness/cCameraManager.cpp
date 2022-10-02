#include "cCameraManager.h"

cCameraManager::cCameraManager()
{
}

cCameraManager::~cCameraManager()
{

}

void cCameraManager::StartUp()
{
	mainCamera = new cFlyCamera();
	mainCamera->Position = glm::vec3(0);
}

void cCameraManager::ShutDown()
{
	delete mainCamera;
}

cFlyCamera* cCameraManager::GetMainCamera()
{
	return mainCamera;
}
