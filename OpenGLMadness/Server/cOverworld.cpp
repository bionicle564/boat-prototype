#include "cOverworld.h"

cOverworld::cOverworld()
{
}

cOverworld::~cOverworld()
{
}

void cOverworld::GenerateIslands(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		sPoint newPoint;
		newPoint.x = rand() % 10;
		newPoint.y = rand() % 10;

		newPoint.type = TYPE::NORMAL;
	}
}
