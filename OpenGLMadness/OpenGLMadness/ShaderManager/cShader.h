#pragma once
#include <glad/glad.h>

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



class cShader
{
private:
	std::map<std::string, GLint> uniformTable;

public:
	GLuint ID;
	
	cShader(std::string vertexFile, std::string fragmentFile);

	void Use();

	void SetBool(std::string name, bool value);
	void SetInt(std::string name, int value);
	void SetFloat(std::string name, float value);

	void SetVec2(std::string name, glm::vec2 value);
	void SetVec2(std::string name, float x, float y);
	
	void SetVec3(std::string name, glm::vec3 value);
	void SetVec3(std::string name, float x, float y, float z);

	void SetVec4(std::string name, glm::vec4 value);
	void SetVec4(std::string name, float x, float y, float z, float w);

	void SetMat2(std::string name, glm::mat2 value);
	void SetMat3(std::string name, glm::mat3 value);
	void SetMat4(std::string name, glm::mat4 value);

};