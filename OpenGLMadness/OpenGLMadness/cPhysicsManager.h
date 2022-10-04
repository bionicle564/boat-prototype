#pragma once
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "cBulletDebugRenderer.h"
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include "cPhysics.h"

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


	std::vector<btRigidBody*> bodies;
	std::vector<btCollisionShape*> shapes;

public:

	cBulletDebugRenderer debugDrawerer;

	cPhysicsManager();
	~cPhysicsManager();
	
	void UpdatePhysics(float dt);

	void StartUp();
	void ShutDown();

	//make a body, adds it to the world and returns the reference
	comp::cPhysics* MakeBody(sBodyDesc desc);

	void DebugDraw();


};