#pragma once
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "cBulletDebugRenderer.h"
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include "cPhysics.h"
#include "cCharacterController.h"
#include "cActionArea.h"


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
	float friction;

	bool kinematic = false;
};


class cPhysicsManager
{
private:

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;


	std::vector<btRigidBody*> bodies;
	std::vector<btCollisionShape*> shapes;
	

public:

	//TODO: put this back to private
	btDiscreteDynamicsWorld* dynamicWorld; //temp placment

	cBulletDebugRenderer debugDrawerer;

	cPhysicsManager();
	~cPhysicsManager();
	
	void UpdatePhysics(float dt);

	void StartUp();
	void ShutDown();

	//make a body, adds it to the world and returns the reference
	comp::cPhysics* MakeBody(sBodyDesc desc);

	comp::cActionArea* MakeActionArea(sBodyDesc desc);

	comp::cCharacterController* MakeController(sBodyDesc desc);
	
	void LinkObjectsPositions(comp::cPhysics* body1, comp::cPhysics* body2);

	void DebugDraw();


};