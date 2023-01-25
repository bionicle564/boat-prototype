#include "cParticleGenerator.h"
#include <glm/gtx/norm.hpp>
#include <algorithm>
#include <iostream>


GLuint comp::cParticleGenerator::nextBindPoint = 0;

comp::cParticleGenerator::cParticleGenerator()
{
	particleMesh = new comp::cMeshRenderer();
	particleMesh->billboard = true;
	particleMesh->meshName = "billboard.fbx";

	this->shader = "defaultParticle"; //TODO: make new shader for discard

	glGenBuffers(1, &this->positionsBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->positionsBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * maxParticles, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, cParticleGenerator::nextBindPoint, this->positionsBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	this->bindPoint = cParticleGenerator::nextBindPoint;

	cParticleGenerator::nextBindPoint++; //there is a better way of doing this

	particlesContainer = new sParticle[maxParticles];
	positions = new glm::vec4[maxParticles];
	
	timer = 0;

}

comp::cParticleGenerator::~cParticleGenerator()
{
	delete particleMesh;
	delete[] particlesContainer;
	delete[] positions;
}

void comp::cParticleGenerator::Update(float dt, glm::vec4 cameraPos, glm::vec4 offset)
{
	int newParticles = 0;
	timer -= dt;
	if (timer <= 0)
	{
		newParticles = 150;
		timer = timeLimit;
	}
	// = (int)std::ceilf(dt * 1); //how many per frame to try and spawn
	//if (newParticles > (int)(0.016f * 10))
	//{
	//	newParticles = (int)(0.016f * 10);
	//}
	
	for (int i = 0; i < newParticles; i++)
	{
		int particleIndex = FindUnusedParticle();
		if (particleIndex != -1)
		{
			particlesContainer[particleIndex].life = 4; //how long it lives
			particlesContainer[particleIndex].position = offset;// +glm::vec4((rand() % (int)halfExtents.x * 2) - (int)halfExtents.x, (rand() % (int)halfExtents.y * 2) - (int)halfExtents.y, (rand() % (int)halfExtents.z * 2) - (int)halfExtents.z, 0); //starting position

			//starting speed
			particlesContainer[particleIndex].velocity = glm::vec4((rand() % 10) / 10.f - .5, (rand() % 10) / 10.f - .5, (rand() % 10) / 10.f - .5, 0); //just something for now
			//particlesContainer[particleIndex].velocity = glm::vec4(0, 1, 0, 0); //just something for now

			particlesContainer[particleIndex].velocity *= 10;
		}
	}
	


 	int particlesCount = 0;

	for (unsigned int i = 0; i < maxParticles; i++)
	{
		sParticle& p = particlesContainer[i];

		if (p.life > 0.0f)
		{
			p.life -= dt;
			if (p.life > 0.0f)
			{
				p.velocity += force * dt;
				p.position += p.velocity * dt;
				p.cameraDistance = glm::length2(p.position - (glm::vec3)cameraPos);

				positions[particlesCount].x = p.position.x;
				positions[particlesCount].y = p.position.y;
				positions[particlesCount].z = p.position.z;
				positions[particlesCount].w = 1;
			}
			else
			{
				p.cameraDistance = -1.f;
			}

			particlesCount++;
		}
	}
	amount = particlesCount;
	
	SortParticles();


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->positionsBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, maxParticles * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particlesCount * sizeof(glm::vec4), positions);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
}

int comp::cParticleGenerator::FindUnusedParticle()
{
	for (unsigned int i = lastUsedParticle; i < maxParticles; i++)
	{
		if (particlesContainer[i].life < 0)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < lastUsedParticle; i++)
	{
		if (particlesContainer[i].life < 0)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	return -1;
}

void comp::cParticleGenerator::SortParticles()
{
	std::sort(&particlesContainer[0], &particlesContainer[maxParticles]);
}

void comp::cParticleGenerator::SetTimer(float time)
{
	timeLimit = time;
	this->timer = timeLimit;
}
