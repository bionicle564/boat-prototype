#include "cPhysicsManager.h"

cPhysicsManager::cPhysicsManager()
{
	dynamicWorld = NULL;

	broadphase = NULL;
	collisionConfiguration = NULL;
	dispatcher = NULL;
	solver = NULL;
}

cPhysicsManager::~cPhysicsManager()
{
	for (int i = 0; i < bodies.size(); i++)
	{
		dynamicWorld->removeRigidBody(bodies[i]);
		delete bodies[i]->getMotionState();
		delete bodies[i];
	}

	delete dynamicWorld;

	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
}

void cPhysicsManager::StartUp()
{
	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;
	this->dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicWorld->setDebugDrawer(&this->debugDrawerer);
	
}
