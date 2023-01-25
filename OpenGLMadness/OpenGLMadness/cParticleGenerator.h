#pragma once
#include "cComponent.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "cMeshRenderer.h"
#include "cEntity.h"

namespace comp
{
	struct sParticle
	{
		glm::vec3 position;
		glm::vec3 velocity;
		//glm::vec4 colour;
		float life;

		float cameraDistance;

		bool operator< (const sParticle& that) const
		{
			return this->cameraDistance > that.cameraDistance;
		}

	};

	class cParticleGenerator : public cComponent
	{
	private:
		
		GLuint positionsBuffer;

		unsigned int lastUsedParticle = 0;

		const unsigned int maxParticles = 1000;
		sParticle* particlesContainer;
		glm::vec4* positions;

		float timeLimit;
		
		//how many seconds between spawning, 0 by default
		float timer;

	public:
		static GLuint nextBindPoint;

		std::string shader;

		GLuint bindPoint;


		unsigned int amount;

		comp::cMeshRenderer* particleMesh;

		//extra force, like gravity or wind
		glm::vec3 force;

		//will spawn particles randomly in a box of the given half dimmensions
		glm::vec3 halfExtents;

		std::string textureName;

		cParticleGenerator();
		~cParticleGenerator();

		void Update(float dt, glm::vec4 cameraPos, glm::vec4 offset = glm::vec4(0,0,0,1));

		int FindUnusedParticle();
		void SortParticles();

		void SetTimer(float time);

		//TODO: implement this; fixing the things that are public
		//void SetParticleTexture(std::string textureName);

	};
}