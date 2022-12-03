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
	~cGameObject(){}




	template<class T> T* AddComponent();

	//if the component was made elsewhere, this takes ownership
	template<class T> void AddComponent(T* component);

	template<class T> T* GetComponent();
	
};



