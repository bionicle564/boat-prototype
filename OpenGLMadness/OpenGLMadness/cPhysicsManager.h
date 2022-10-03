#pragma once
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "cBulletDebugRenderer.h"
#include <vector>
#include <glm/gtc/quaternion.hpp>

enum eBodyType
{
	BOX,
	SPHERE,
	CAPSULE
};

struct sBodyDesc
{
	eBodyType type;
	glm::vec3 position;
	glm::quat orientation;
	glm::vec4 halfExtents;

	float mass;
};


class cPhysicsManager
{
private:
	btDiscreteDynamicsWorld* dynamicWorld;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	cBulletDebugRenderer debugDrawerer;

	std::vector<btRigidBody*> bodies;
	std::vector<btCollisionShape*> shapes;

public:


	cPhysicsManager();
	~cPhysicsManager();

	void StartUp();
	void ShutDown();

	//make a body, adds it to the world and returns the reference
	btRigidBody* MakeBody(sBodyDesc desc);



};