#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include "sVertexData.h"

struct sMeshData
{
	std::string filename;
	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	unsigned int nBones;

	glm::vec4 specInfo;


	//std::vector<aiMatrix4x4> boneMatrices;
	//std::vector<glm::mat4> boneMats;

	//add the info into the first available spot
	void AddBoneInfo(int vertexId, int boneId, float weight)
	{

		if (pVertices[vertexId].bId1 == -1)
		{
			pVertices[vertexId].bId1 = boneId;
			pVertices[vertexId].bWeight1 = weight;
			return;
		}
		else if (pVertices[vertexId].bId2 == -1)
		{
			pVertices[vertexId].bId2 = boneId;
			pVertices[vertexId].bWeight2 = weight;
			return;
		}
		else if (pVertices[vertexId].bId3 == -1)
		{
			pVertices[vertexId].bId3 = boneId;
			pVertices[vertexId].bWeight3 = weight;
			return;
		}
		else if (pVertices[vertexId].bId4 == -1)
		{
			pVertices[vertexId].bId4 = boneId;
			pVertices[vertexId].bWeight4 = weight;
			return;
		}

	}

	std::vector<sVertexData> pVertices;
	std::vector<unsigned int> pIndices;

};