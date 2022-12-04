#include "cShader.h"
#include <fstream>
#include <sstream>
#include <iostream>

cShader::cShader(std::string vertexFile, std::string fragmentFile)
{
	std::string vertexSource;
	std::string fragmentSource;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);

		std::stringstream vShaderStream, fShaderStream;
		
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexSource = vShaderStream.str();
		fragmentSource = fShaderStream.str();


	}
	catch (std::exception e)
	{
		std::cout << "File not found/read" << std::endl;
	}

	const char* vShaderCode = vertexSource.c_str();
	const char* fShaderCode = fragmentSource.c_str();

	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	//create the program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//clean up
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

cShader::cShader(std::string vertexFile, std::string fragmentFile, std::string geomatryFile)
{
	std::string vertexSource;
	std::string fragmentSource;
	std::string geoSource;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);
		gShaderFile.open(geomatryFile);

		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();

		// convert stream into string
		vertexSource = vShaderStream.str();
		fragmentSource = fShaderStream.str();
		geoSource = gShaderStream.str();


	}
	catch (std::exception e)
	{
		std::cout << "File not found/read" << std::endl;
	}

	const char* vShaderCode = vertexSource.c_str();
	const char* fShaderCode = fragmentSource.c_str();
	const char* gShaderCode = geoSource.c_str();

	GLuint vertex, fragment, geometry;
	int success;
	char infoLog[512];

	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//geomatry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//create the program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//clean up
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

void cShader::Use()
{
	glUseProgram(ID);
}

void cShader::SetBool(std::string name, bool value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}

	glUniform1i(uniformTable[name], (int)value);
}

void cShader::SetInt(std::string name, int value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
 		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}


	glUniform1i(uniformTable[name], value);
}

void cShader::SetFloat(std::string name, float value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}

	glUniform1f(uniformTable[name], value);
}

void cShader::SetVec2(std::string name, glm::vec2 value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}
	glUniform2fv(uniformTable[name], 1, &value[0]);
}

void cShader::SetVec2(std::string name, float x, float y)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}
	glUniform2f(uniformTable[name], x, y);
}

void cShader::SetVec3(std::string name, glm::vec3 value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}

	glUniform3fv(uniformTable[name], 1, &value[0]);
}

void cShader::SetVec3(std::string name, float x, float y, float z)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}

	glUniform3f(uniformTable[name], x, y, z);
}

void cShader::SetVec4(std::string name, glm::vec4 value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}

	glUniform4fv(uniformTable[name], 1, &value[0]);
}

void cShader::SetVec4(std::string name, float x, float y, float z, float w)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}

	glUniform4f(uniformTable[name], x, y, z, w);
}

void cShader::SetMat2(std::string name, glm::mat2 value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}
	glUniformMatrix2fv(uniformTable[name], 1, GL_FALSE, &value[0][0]);
}

void cShader::SetMat3(std::string name, glm::mat3 value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}
	glUniformMatrix3fv(uniformTable[name], 1, GL_FALSE, &value[0][0]);
}

void cShader::SetMat4(std::string name, glm::mat4 value)
{
	if (uniformTable.find(name) == uniformTable.end())
	{
		uniformTable[name] = glGetUniformLocation(ID, name.c_str());
	}
	glUniformMatrix4fv(uniformTable[name], 1, GL_FALSE, &value[0][0]);
}

