#pragma once
#include <vector>


enum TYPE
{
	NORMAL,
	OUTPOST
};

struct sPoint
{
	int x;
	int y;
	TYPE type;
};

class cOverworld
{
public:
	cOverworld();
	~cOverworld();

	std::vector<sPoint> points;
	
	//doesn't actualy make islands right now, just where they would be in the overworld
	void GenerateIslands(int amount);

};