#pragma once

#include "cEntity.h"
#include <glm/vec3.hpp>
#include "cGameEngine.h"


class cPlayer : public cEntity
{
private:
	float deltaTime;

	float walkSpeed;

public:


	btRigidBody* bodySelfRef;

	cPlayer();
	virtual ~cPlayer();

	void SetUp(cGameEngine& engine);

	void Update(float dt);

	void WalkUp();
	void WalkDown();
	void WalkLeft();
	void WalkRight();

	void SetSpeed(btVector3 vec);
	void SetSpeed(glm::vec3 vec);

};