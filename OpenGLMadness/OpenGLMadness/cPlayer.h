#pragma once

#include "cEntity.h"
#include <glm/vec3.hpp>


class cPlayer : cEntity
{
private:
	glm::vec3 boatPosition;
	bool linkBoatPosition;


public:
	cPlayer();
	virtual ~cPlayer();



};