#include "cRaft.h"
#include "cPhysics.h"
#include <btBulletDynamicsCommon.h>

cRaft::cRaft()
{
}

cRaft::~cRaft()
{
}

std::vector<cPart*>& cRaft::GetBoatParts()
{
	// TODO: insert return statement here
	return this->parts;
}

void cRaft::Update(float deltaTime)
{

	btRigidBody* rb = this->ent->GetComponent<comp::cPhysics>()->rb;

	//simple boat movment
	btTransform newTrans;
	rb->getMotionState()->getWorldTransform(newTrans);
	newTrans.getOrigin() += (btVector3(.3f, 0, 0) * deltaTime);
	rb->getMotionState()->setWorldTransform(newTrans);
}
