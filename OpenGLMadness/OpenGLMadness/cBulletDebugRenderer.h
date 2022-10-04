#pragma once
#include "GLCommon.h"
#include <btBulletCollisionCommon.h>
#include <glm/common.hpp>
#include <glm/mat4x4.hpp>

/*
	Debug renderer taken from https://github.com/opengl-tutorials/ogl/blob/master/misc05_picking/misc05_picking_BulletPhysics.cpp
	
*/



class cBulletDebugRenderer : public btIDebugDraw{
public:
	glm::mat4 pViewMatrix;
	glm::mat4 pProjectionMatrix;
	bool first = true;
	GLuint buffer;
	
	GLuint VAO;

	GLint program;


	void SetView(glm::mat4 mat) 
	{
		pViewMatrix = mat;
	}

	void SetProj(glm::mat4 mat)
	{
		pProjectionMatrix = mat;
	}

	void SetMatrices()
	{

	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual void drawContactPoint(const btVector3 &,const btVector3 &,btScalar,int,const btVector3 &){}
	virtual void reportErrorWarning(const char *){}
	virtual void draw3dText(const btVector3 &,const char *){}
	virtual void setDebugMode(int p){
		m = p;
	}
	int getDebugMode(void) const {return m;}
	int m;
};