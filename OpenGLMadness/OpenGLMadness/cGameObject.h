#pragma once
#include "cEntity.h"
#include "cEntityManager.h"



class cGameObject
{
private:
	cEntity* ent;
	

public:
	static cEntityManager* factory;
	static void SetFactory(cEntityManager* fac);


	cGameObject();
	virtual ~cGameObject(){}


	template<class T> T* AddComponent()
	{
		return this->ent->AddComponent<T>();
	}

	//if the component was made elsewhere, this takes ownership
	template<class T> void AddComponent(T* component) 
	{
		this->ent->AddComponent(component);
	}

	template<class T> T* GetComponent()
	{
		return this->ent->GetComponent<T>();
	}
	
};



