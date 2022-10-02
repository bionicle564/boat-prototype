#include "cTextureManager.h"
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

cTextureManager::cTextureManager()
{
}

cTextureManager::~cTextureManager()
{
}

void cTextureManager::StartUp()
{
	folderPath = "Assets\\Textures\\";
	LoadTexture2D("default.png");
}

void cTextureManager::ShutDown()
{
}

bool cTextureManager::LoadTexture2D(std::string fileName, std::string friendlyName)
{
	std::stringstream ss;
	ss << folderPath << fileName;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//simple settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(ss.str().c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}

	stbi_image_free(data);

	if (textures.find(friendlyName) == textures.end())
	{
		textures[friendlyName] = texture;
	}
	else
	{
		std::string cutFile = fileName.substr(0, fileName.find('.')); //use the name of the file without the extension
		textures[cutFile] = texture;
	}


	return true;
}

GLuint cTextureManager::GetTexture(std::string name)
{
	if (textures.find(name) != textures.end())
	{
		return textures[name];
	}

	return textures["default"];
}
