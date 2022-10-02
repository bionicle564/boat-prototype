#pragma once
#include "cEntity.h"

class cUIEntity : public cEntity
{
private:

	cUIEntity* parent;
	std::vector<cUIEntity*> children;

public:
	int x;
	int y;

	int width;
	int height;

	float screenX;
	float screenY;

	bool hide = false;

	cUIEntity(cUIEntity* parent);
	~cUIEntity();
	

	void AddChild(cUIEntity* c);
	const std::vector<cUIEntity*>& GetChildren();
};