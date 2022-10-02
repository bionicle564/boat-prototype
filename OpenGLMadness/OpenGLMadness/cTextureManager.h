#pragma once
#include <map>
#include "GLCommon.h"
#include <string>

class cTextureManager
{
private:
	std::map<std::string, GLuint> textures;
	std::string folderPath;

public:

	cTextureManager();
	~cTextureManager();

	void StartUp();
	void ShutDown();

	bool LoadTexture2D(std::string fileName, std::string friendlyName = "default");

	GLuint GetTexture(std::string name);
};