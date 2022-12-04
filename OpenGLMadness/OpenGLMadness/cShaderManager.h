#pragma once
#include "ShaderManager/cShader.h"
#include <string>
#include <map>


class cShaderManager
{
private:
	std::map<std::string, cShader*> nameToShader;

	cShader* currentShader;

	std::string folderPath;

	std::string nameOfCurrentShader;

	

public:

	cShaderManager();
	~cShaderManager();

	void StartUp();
	void ShutDown();

	void SetShaderPath(std::string folderPath);

	bool MakeShader(std::string vertexPath, std::string fragmentPath, std::string shaderName);
	bool MakeShader(std::string vertexPath, std::string fragmentPath, std::string geoPath, std::string shaderName);

	cShader* GetCurrentShader();
	cShader* GetShaderFromName(std::string name);
	std::string GetCurrentShaderName();
	GLint GetIDFromName(std::string name);

	void UseShader(std::string shaderName);

	unsigned int uboMatId;
};