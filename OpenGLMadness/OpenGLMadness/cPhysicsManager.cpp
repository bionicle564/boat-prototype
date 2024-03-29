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

void cPhysicsManager::UpdatePhysics(float dt)
{
	dynamicWorld->stepSimulation(dt);
}

void cPhysicsManager::StartUp()
{
	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;
	this->dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	debugDrawerer.setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawAabb);

	dynamicWorld->setDebugDrawer(&this->debugDrawerer);
	dynamicWorld->setGravity(btVector3(0, -9.8f, 0));
}

void cPhysicsManager::ShutDown()
{
}

comp::cPhysics* cPhysicsManager::MakeBody(sBodyDesc desc)
{
	btVector3 inertia;
	btCollisionShape* shape = NULL;
	if (desc.type == eBodyType::BOX)
	{
		shape = new btBoxShape(btVector3(desc.halfExtents.x, desc.halfExtents.y, desc.halfExtents.z));
	}
	else if (desc.type == eBodyType::SPHERE)
	{
		shape = new btSphereShape(desc.halfExtents.x);
	}
	else if (desc.type == eBodyType::CAPSULE)
	{
		shape = new btCapsuleShape(desc.halfExtents.x, desc.halfExtents.y);
	}
	else
	{
		return NULL;
	}


	shapes.push_back(shape); //keep track of it to delete later

	btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
		btQuaternion(desc.orientation.x, desc.orientation.y, desc.orientation.z, desc.orientation.w),
		btVector3(desc.position.x, desc.position.y, desc.position.z)
	));

	shape->calculateLocalInertia(desc.mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		desc.mass,                  // mass, in kg. 0 -> Static object, will never move.
		motionstate,
		shape,  // collision shape of body
		inertia    // local inertia
	);

	btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setActivationState(ACTIVE_TAG);

	//rigidBody->setMassProps(desc.mass,inertia);
	rigidBody->setFriction(desc.friction);

	bodies.push_back(rigidBody);
	dynamicWorld->addRigidBody(rigidBody);

	comp::cPhysics* component = new comp::cPhysics();

	component->rb = rigidBody;
	return component;
}

comp::cCharacterController* cPhysicsManager::MakeController(sBodyDesc desc)
{
	comp::cCharacterController* charCon = new comp::cCharacterController();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(desc.position.x, desc.position.y, desc.position.z));
	startTransform.setRotation(btQuaternion(desc.orientation.x, desc.orientation.y, desc.orientation.z, desc.orientation.w));

	btConvexShape* shape = new btCapsuleShape(desc.halfExtents.x, desc.halfExtents.y);

	shapes.push_back(shape);

	charCon->ghostObject = new btPairCachingGhostObject();
	charCon->ghostObject->setWorldTransform(startTransform);
	

	dynamicWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	
	charCon->ghostObject->setCollisionShape(shape);
	charCon->ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	
	

	charCon->charCon = new btKinematicCharacterController(charCon->ghostObject, shape, 0.05f, btVector3(0,1,0));
	charCon->charCon->setGravity(dynamicWorld->getGravity());

	dynamicWorld->addCollisionObject(charCon->ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
	dynamicWorld->addAction(charCon->charCon);
	charCon->charCon->setMaxJumpHeight(1.5);
	

	return charCon;
}

void cPhysicsManager::DebugDraw()
{
	this->dynamicWorld->debugDrawWorld();
}
