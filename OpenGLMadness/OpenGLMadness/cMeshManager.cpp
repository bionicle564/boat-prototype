#include "GLCommon.h"
#include "cMeshManager.h"
#include <sstream>
#include <assimp/scene.h>
#include <assimp/Vertex.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

cMeshManager::cMeshManager()
{
}

cMeshManager::~cMeshManager()
{
}

void cMeshManager::StartUp()
{
	folderPath = "Assets\\Models\\";
	importer = new Assimp::Importer();
}

void cMeshManager::ShutDown()
{
	delete importer;
}

void cMeshManager::SetFilePath(std::string folderPath)
{
	this->folderPath = folderPath;
}

bool cMeshManager::LoadMesh(std::string fileName)
{
	if (this->nameToId.find(fileName) != this->nameToId.end())
	{
		return true;
	}

	std::stringstream ss;
	ss << folderPath << fileName;

	const aiScene* root = importer->ReadFile(ss.str(), aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);


	if (root->HasMeshes())
	{
		for (unsigned int i = 0; i < root->mNumMeshes; i++)
		{
			sMeshData info;

			aiMesh* temp = root->mMeshes[i];

			for (unsigned int j = 0; j < temp->mNumVertices; j++)
			{
				sVertexData vData;

				vData.bId1 = -1;
				vData.bId2 = -1;
				vData.bId3 = -1;
				vData.bId4 = -1;
				
				vData.bWeight1 = 0.0;
				vData.bWeight2 = 0.0;
				vData.bWeight3 = 0.0;
				vData.bWeight4 = 0.0;

				vData.x = temp->mVertices[j].x;
				vData.y = temp->mVertices[j].y;
				vData.z = temp->mVertices[j].z;
				vData.w = 1.0f;

				if (temp->HasNormals())
				{
					vData.nx = temp->mNormals[j].x;
					vData.ny = temp->mNormals[j].y;
					vData.nz = temp->mNormals[j].z;
					vData.nw = 1;
				}
				

				if (temp->mTextureCoords[0])
				{
					vData.u0 = temp->mTextureCoords[0][j].x;
					vData.v0 = temp->mTextureCoords[0][j].y;
					vData.u1 = 0.0f;
					vData.v1 = 0.0f;
				}
				else
				{
					vData.u0 = 1;
					vData.v0 = 1;
					vData.u1 = 1;
					vData.v1 = 1;

				}

				//TODO: Tangents and Bitangents

				info.pVertices.push_back(vData);

			}

			info.numberOfVertices = temp->mNumVertices;
			info.numberOfIndices = temp->mNumFaces * 3;
			info.numberOfTriangles = temp->mNumFaces;

			info.pIndices.resize(info.numberOfIndices);

			unsigned int index = 0;
			for (int j = 0; j < temp->mNumFaces; j++)
			{
				aiFace tempFace = temp->mFaces[j];
				for (int k = 0; k < tempFace.mNumIndices; k++)
				{
					info.pIndices[index + k] = tempFace.mIndices[k];
				}
				index += tempFace.mNumIndices;
			}

			info.VertexBuffer_Start_Index = 0;
			info.IndexBuffer_Start_Index = 0;

			this->idToMeshes[nextID].push_back(info);
		}
	}

	for (int i = 0; i < this->idToMeshes[nextID].size(); i++)
	{
		sMeshData& drawInfo = this->idToMeshes[nextID][i];
		glGenVertexArrays(1, &(drawInfo.VAO_ID));

		glBindVertexArray(drawInfo.VAO_ID);

		glGenBuffers(1, &(drawInfo.VertexBufferID));
		glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sVertexData) * drawInfo.numberOfVertices, (GLvoid*)drawInfo.pVertices.data(), GL_STATIC_DRAW);


		glGenBuffers(1, &(drawInfo.IndexBufferID));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * drawInfo.numberOfIndices, (GLvoid*)drawInfo.pIndices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sVertexData), (void*)offsetof(sVertexData, x)); 
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(sVertexData), (void*)offsetof(sVertexData, nx));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sVertexData), (void*)offsetof(sVertexData, u0));
		glEnableVertexAttribArray(2);



		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}


	nameToId[fileName] = nextID;

	nextID++;
	return true;
}

bool cMeshManager::GetMeshByName(std::string name, std::vector<sMeshData>& rtn)
{
	rtn = idToMeshes[nameToId[name]];
	return true;
}
