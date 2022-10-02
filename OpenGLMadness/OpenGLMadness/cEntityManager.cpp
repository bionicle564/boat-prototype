#include "cEntityManager.h"
#include "cMeshRenderer.h"

cEntityManager::cEntityManager()
{
}

cEntityManager::~cEntityManager()
{
}

void cEntityManager::StartUp()
{
	masterCanvas = new cUIEntity(NULL);
	cUIEntity* ui = CreateUIEnitity();
	ui->hide = true;
	ui->width = 20;
	ui->height = 30;

	this->cursor = ui;

}

void cEntityManager::ShutDown()
{
	for (std::vector<cEntity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		delete* it;
	}


	delete masterCanvas;
}

const std::vector<cEntity*>& cEntityManager::GetEntities()
{
	return this->entities;
}

cEntity* cEntityManager::CreateEntity()
{
	cEntity* newEntity = new cEntity();
	entities.push_back(newEntity);
	return newEntity;
}


cUIEntity* cEntityManager::CreateUIEnitity(cUIEntity* parent)
{
	cUIEntity* newUI = new cUIEntity(parent);
	parent->AddChild(newUI);

	return newUI;
}

cUIEntity* cEntityManager::CreateUIEnitity()
{
	cUIEntity* newUI = new cUIEntity(this->masterCanvas);
	this->masterCanvas->AddChild(newUI);
	newUI->AddComponent<comp::cMeshRenderer>()->meshName = "base_ui.fbx";

	return newUI;
}


