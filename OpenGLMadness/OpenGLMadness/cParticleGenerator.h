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
		glm::vec4 position;
		glm::vec4 velocity;
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

		const unsigned int maxParticles = 100000;
		sParticle* particlesContainer;
		glm::vec4* positions;

		//texture


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


		cParticleGenerator();
		~cParticleGenerator();

		void Update(float dt, glm::vec4 cameraPos, glm::vec4 offset = glm::vec4(0,0,0,1));

		int FindUnusedParticle();
		void SortParticles();

		//TODO: implement this; fixing the things that are public
		//void SetParticleTexture(std::string textureName);

	};
}