#include "cPlayer.h"
#include "cScale.h"
#include "cPosition.h"
#include "cRotation.h"


cPlayer::cPlayer()
{
}

cPlayer::~cPlayer()
{
}

void cPlayer::SetUp(cGameEngine& engine)
{
	this->AddComponent<comp::cMeshRenderer>()->meshName = "capsule.fbx";
	this->AddComponent<comp::cScale>()->scale = glm::vec3(.65,.65,.80);
	//addComponent<comp::cScale>()->scale = glm::vec3(0);
	this->AddComponent<comp::cPosition>();
	this->AddComponent<comp::cRotation>()->rotation = glm::quat(glm::vec3(glm::half_pi<float>(), 0, 0));
	

	sBodyDesc desc;
	desc.halfExtents = glm::vec4(.75, 2, 0, 0);
	desc.mass = 1;
	desc.position = glm::vec3(0, 3, -10);
	desc.type = eBodyType::CAPSULE;
	desc.orientation = glm::quat(glm::vec3(0));
	//desc.orientation = glm::quat(glm::vec3(glm::half_pi<float>(),0,0));
	desc.friction = 1;
	
	this->AddComponent(engine.physicsManager.MakeBody(desc));
	this->bodySelfRef = this->GetComponent<comp::cPhysics>()->rb;
	this->bodySelfRef->setAngularFactor(btVector3(0, 0, 0));
	//dude->AddComponent(engine.physicsManager.MakeController(desc));
}

void cPlayer::Update(float dt)
{
}

void cPlayer::WalkUp()
{
}

void cPlayer::WalkDown()
{
}

void cPlayer::WalkLeft()
{
}

void cPlayer::WalkRight()
{
}

void cPlayer::SetSpeed(btVector3 vec)
{

	this->bodySelfRef->setActivationState(ACTIVE_TAG);
	this->bodySelfRef->setLinearVelocity(vec);
}

void cPlayer::SetSpeed(glm::vec3 vec)
{
	this->bodySelfRef->setActivationState(ACTIVE_TAG);
	this->bodySelfRef->setLinearVelocity(btVector3(vec.x, vec.y, vec.z));
}


