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

	for (int i = 0; i < shapes.size(); i++)
	{
		delete shapes[i];
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

btRigidBody* cPhysicsManager::MakeBody(sBodyDesc desc)
{
	btCollisionShape* shape = NULL;
	if (desc.type == eBodyType::BOX)
	{
		shape = new btBoxShape(btVector3(desc.halfExtents.x, desc.halfExtents.y, desc.halfExtents.z));
	}
	else if (desc.type == eBodyType::SPHERE)
	{

	}
	else if (desc.type == eBodyType::CAPSULE)
	{

	}

	shapes.push_back(shape); //keep track of it to delete later

	btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
		btQuaternion(desc.orientation.x, desc.orientation.y, desc.orientation.z, desc.orientation.w),
		btVector3(desc.position.x, desc.position.y, desc.position.z)
	));

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		desc.mass,                  // mass, in kg. 0 -> Static object, will never move.
		motionstate,
		shape,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);

	btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

	bodies.push_back(rigidBody);
	dynamicWorld->addRigidBody(rigidBody);

	return rigidBody;
}
