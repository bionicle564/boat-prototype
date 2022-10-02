#pragma once
#include "cEntity.h"
#include "cUIEntity.h"
#include <vector>


class cEntityManager
{
private:
	std::vector<cEntity*> entities;
	//std::vector<cEntity*> uiElements;

public:

	cUIEntity* masterCanvas;
	cUIEntity* cursor;

	cEntityManager();
	~cEntityManager();

	void StartUp();
	void ShutDown();

	const std::vector<cEntity*>& GetEntities();
	cEntity* CreateEntity();

	cUIEntity* CreateUIEnitity(cUIEntity* parent);
	cUIEntity* CreateUIEnitity();

};