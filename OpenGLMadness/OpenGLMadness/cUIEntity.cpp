#include "cUIEntity.h"

cUIEntity::cUIEntity(cUIEntity* parent)
{
	x = 0;
	y = 0;
	width = 10;
	height = 10;

	screenX = -1.f;
	screenY = -1.f;

	this->parent = parent;
}

cUIEntity::~cUIEntity()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void cUIEntity::AddChild(cUIEntity* c)
{
	children.push_back(c);
}

const std::vector<cUIEntity*>& cUIEntity::GetChildren()
{
	return this->children;
}
