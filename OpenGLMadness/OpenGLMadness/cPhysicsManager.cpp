#include "cPhysicsManager.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>


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
	//removed/delete constraints
	for (int i = dynamicWorld->getNumConstraints() - 1; i >= 0; i--)
	{
		btTypedConstraint* constraint = dynamicWorld->getConstraint(i);
		dynamicWorld->removeConstraint(constraint);
		delete constraint;
	}

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

	for (int i = 0; i < zones.size(); i++)
	{
		dynamicWorld->removeCollisionObject(zones[i]);
		delete zones[i];
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

	if (desc.kinematic)
	{
		rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
		rigidBody->setActivationState(DISABLE_DEACTIVATION);
	}

	//rigidBody->setMassProps(desc.mass,inertia);
	rigidBody->setFriction(desc.friction);
	rigidBody->setRestitution(0);

	bodies.push_back(rigidBody);
	dynamicWorld->addRigidBody(rigidBody);

	comp::cPhysics* component = new comp::cPhysics();

	component->rb = rigidBody;
	return component;
}

comp::cActionArea* cPhysicsManager::MakeActionArea(sBodyDesc desc)
{
	comp::cActionArea* area = new comp::cActionArea();

	btGhostObject* ghostObject = new btGhostObject();
	ghostObject->setCollisionFlags(4);

	ghostObject->setCollisionShape(new btBoxShape(btVector3(desc.halfExtents.x, desc.halfExtents.y, desc.halfExtents.z)));
	ghostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
	this->dynamicWorld->addCollisionObject(ghostObject);

	this->dynamicWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	
	zones.push_back(ghostObject);

	area->area = ghostObject;

	return area;
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

void cPhysicsManager::LinkObjectsPositions(comp::cPhysics* body1, comp::cPhysics* body2)
{
	btRigidBody* rb1 = body1->rb;
	btRigidBody* rb2 = body2->rb;

	body2->rb->setAngularFactor(btVector3(0,0,0));
	

	body2->rb->setActivationState(DISABLE_DEACTIVATION);

	btVector3 position1 = rb1->getWorldTransform().getOrigin();
	btVector3 position2 = rb2->getWorldTransform().getOrigin();


	btVector3 position = position2;//position1 - position2;
	//position /= 2;

	btTypedConstraint* p2p = new btPoint2PointConstraint(*rb1, *rb2, position1, position2);
	//btTypedConstraint* p2p = new btPoint2PointConstraint(*rb1, *rb2, btVector3(0,0,0), rb1->getWorldTransform().getOrigin());
	p2p->setBreakingImpulseThreshold(3);
	dynamicWorld->addConstraint(p2p);
	
}

void cPhysicsManager::DebugDraw()
{
	this->dynamicWorld->debugDrawWorld();
}
