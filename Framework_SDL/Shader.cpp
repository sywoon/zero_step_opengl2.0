#include "Shader.h"
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"


static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram,
	const std::string& errMsg);

static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);



Shader::Shader(const std::string & fileName)
{
	_program = glCreateProgram();
	_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(_program, _shaders[i]);
	}

	glBindAttribLocation(_program, 0, "position");
	glBindAttribLocation(_program, 1, "texCoord");

	glLinkProgram(_program);
	CheckShaderError(_program, GL_LINK_STATUS, true, "Error:Program link failed");

	glValidateProgram(_program);
	CheckShaderError(_program, GL_VALIDATE_STATUS, true, "Error:Program validate failed");

	_uniforms[TRANSFORM_U] = glGetUniformLocation(_program, "transform");
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(_program, _shaders[i]);
		glDeleteShader(_shaders[i]);
	}
	glDeleteProgram(_program);
}

void Shader::Use()
{
	glUseProgram(_program);
}

void Shader::Update(const Transform & transform, const Camera& camera)
{
	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}







static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram,
	const std::string& errMsg)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		std::cerr << errMsg << ":'" << error << "'" << std::endl;
	}
}


static std::string LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader:" << fileName << std::endl;
	}

	return output;
}

static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		std::cerr << "Error:shader create failure:" << text << std::endl;
	}

	const GLchar* shaderSourceString[1];
	GLint shaderSourceStringLength[1];
	shaderSourceString[0] = text.c_str();
	shaderSourceStringLength[0] = text.length();

	glShaderSource(shader, 1, shaderSourceString, shaderSourceStringLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error:Shader compile failed");

	return shader;
}