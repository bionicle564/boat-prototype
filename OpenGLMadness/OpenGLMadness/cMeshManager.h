#pragma once
#include <string>
#include <map>
#include <vector>

#include "sMeshData.h"

#include <assimp/Importer.hpp>

class cMeshManager
{
private:
	unsigned int nextID;

	std::string folderPath;
	std::map<unsigned int, std::vector<sMeshData>> idToMeshes;
	std::map<std::string, unsigned int> nameToId;

	Assimp::Importer* importer;

public:
	cMeshManager();
	~cMeshManager();

	void StartUp();
	void ShutDown();

	void SetFilePath(std::string folderPath);

	bool LoadMesh(std::string fileName);
	bool GetMeshByName(std::string name, std::vector<sMeshData>& rtn);



};