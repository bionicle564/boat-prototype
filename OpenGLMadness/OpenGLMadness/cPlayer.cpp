#include "cPlayer.h"
#include "cScale.h"
#include "cPosition.h"

cPlayer::cPlayer()
{
}

cPlayer::~cPlayer()
{
}

void cPlayer::SetUp(cGameEngine& engine)
{
	this->AddComponent<comp::cMeshRenderer>()->meshName = "fixed_knight.fbx";
	this->AddComponent<comp::cScale>()->scale = glm::vec3(1);
	this->AddComponent<comp::cPosition>();


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
