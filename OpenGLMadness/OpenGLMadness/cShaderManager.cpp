#include "cShaderManager.h"
#include <sstream>
#include "cParticleGenerator.h"

cShaderManager::cShaderManager()
{
}

cShaderManager::~cShaderManager()
{
}

void cShaderManager::StartUp()
{
	SetShaderPath("Assets\\Shaders\\");
	MakeShader("vert.glsl", "frag.glsl", "default");
	MakeShader("vert.glsl", "borderColour.glsl", "singleColour");
	MakeShader("uiVert.glsl", "borderColour.glsl", "simpleUI");
	MakeShader("partVert.glsl", "borderColour.glsl", "defaultParticle");
	MakeShader("simpleVert.glsl", "simpleFrag.glsl", "debugPhysics");

	UseShader("default");


	glGenBuffers(1, &uboMatId);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatId);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatId, 0, 2 * sizeof(glm::mat4));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);


}

void cShaderManager::ShutDown()
{
	//clean up all shaders
	for (std::map<std::string, cShader*>::iterator it = nameToShader.begin(); it != nameToShader.end(); it++)
	{
		delete it->second;
	}
}

void cShaderManager::SetShaderPath(std::string folderPath)
{
	this->folderPath = folderPath;
}

bool cShaderManager::MakeShader(std::string vertexPath, std::string fragmentPath, std::string shaderName)
{
	std::stringstream vertex;
	vertex << this->folderPath << vertexPath;

	std::stringstream fragment;
	fragment << this->folderPath << fragmentPath;

	cShader* newShader = new cShader(vertex.str(), fragment.str());
	
	GLuint matsLoc = glGetUniformBlockIndex(newShader->ID, "MatrixBlock");
	if (matsLoc != GL_INVALID_INDEX)
	{
		glUniformBlockBinding(newShader->ID, matsLoc, 0);
	}

	//set this in the renderer at run time
	//GLuint offsLoc = glGetUniformBlockIndex(newShader->ID, "OffsetsBlock");
	//if (offsLoc != GL_INVALID_INDEX)
	//{
	//	glShaderStorageBlockBinding(newShader->ID, offsLoc, 0);
	//}
	

	nameToShader[shaderName] = newShader;


	return true;
}

bool cShaderManager::MakeShader(std::string vertexPath, std::string fragmentPath, std::string geoPath, std::string shaderName)
{
	std::stringstream vertex;
	vertex << this->folderPath << vertexPath;

	std::stringstream fragment;
	fragment << this->folderPath << fragmentPath;

	std::stringstream geo;
	geo << this->folderPath << geoPath;

	cShader* newShader = new cShader(vertex.str(), fragment.str(), geo.str());

	GLuint matsLoc = glGetUniformBlockIndex(newShader->ID, "MatrixBlock");
	if (matsLoc != GL_INVALID_INDEX)
	{
		glUniformBlockBinding(newShader->ID, matsLoc, 0);
	}

	//set this in the renderer at run time
	//GLuint offsLoc = glGetUniformBlockIndex(newShader->ID, "OffsetsBlock");
	//if (offsLoc != GL_INVALID_INDEX)
	//{
	//	glShaderStorageBlockBinding(newShader->ID, offsLoc, 0);
	//}


	nameToShader[shaderName] = newShader;


	return true;
}

cShader* cShaderManager::GetCurrentShader()
{
	return currentShader;
}

cShader* cShaderManager::GetShaderFromName(std::string name)
{
	cShader* shader = NULL;

	if (this->nameToShader.find(name) != this->nameToShader.end())
	{
		shader = nameToShader[name];
	}

	return shader;
}

std::string cShaderManager::GetCurrentShaderName()
{
	return nameOfCurrentShader;
}

GLint cShaderManager::GetIDFromName(std::string name)
{
	GLint program = -1;

	if (this->nameToShader.find(name) != this->nameToShader.end())
	{
		program = nameToShader[name]->ID;
	}

	return program;
}

void cShaderManager::UseShader(std::string shaderName)
{
	nameOfCurrentShader = shaderName;
	currentShader = nameToShader[shaderName];
	currentShader->Use();
}
