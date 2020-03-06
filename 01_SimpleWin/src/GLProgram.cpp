#include "GLProgram.h"
#include "Util.h"
#include <string>


GLProgram::GLProgram()
	: _uProgram(0)
	, _uVertShader(0)
	, _uFragShader(0)
{
	memset(_uUniforms, 0, sizeof(_uUniforms));
}

GLProgram::~GLProgram()
{
	if (_uProgram)
	{
		glDeleteProgram(_uProgram);
	}
}


bool GLProgram::initWithVertexShaderFilename(const char* vShaderFilename, const char* fShaderFilename)
{
	std::string vs, fs;
	if (!ReadFile(vShaderFilename, vs))
	{
		return false;
	}

	if (!ReadFile(fShaderFilename, fs))
	{
		return false;
	}
	
	return initWithVertexShaderByteArray(vs.c_str(), fs.c_str());
}

bool GLProgram::initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
	_uProgram = glCreateProgram();
	if (0 == _uProgram)
	{
		ErrorOut("create program failed");
		return false;
	}
	CHECK_GL_ERROR_DEBUG();
	
	_uVertShader = _uFragShader = 0;
	
	if (vShaderByteArray)
	{
		if (!compileShader(&_uVertShader, GL_VERTEX_SHADER, vShaderByteArray))
		{
			Log("cocos2d: ERROR: Failed to compile vertex shader");
			return false;
		}
	}
	CHECK_GL_ERROR_DEBUG();

	if (fShaderByteArray)
	{
		if (!compileShader(&_uFragShader, GL_FRAGMENT_SHADER, fShaderByteArray))
		{
			Log("cocos2d: ERROR: Failed to compile fragment shader");
			return false;
		}
	}
	CHECK_GL_ERROR_DEBUG();

	if (_uVertShader)
	{
		glAttachShader(_uProgram, _uVertShader);
	}
	CHECK_GL_ERROR_DEBUG();

	if (_uFragShader)
	{
		glAttachShader(_uProgram, _uFragShader);
	}

	CHECK_GL_ERROR_DEBUG();

	return true;
}


bool GLProgram::compileShader(GLuint* shader, GLenum type, const GLchar* source)
{
	GLint status;

	if (!source)
		return false;

	const GLchar* sources[] = {
		source,
	};

	*shader = glCreateShader(type);
	glShaderSource(*shader, sizeof(sources) / sizeof(*sources), sources, NULL);
	glCompileShader(*shader);

	glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar*)malloc(sizeof(GLchar) * length);

		glGetShaderSource(*shader, length, NULL, src);
		Log("cocos2d: ERROR: Failed to compile shader:%s\n", src);
		free(src);

		char* logBytes = type == GL_VERTEX_SHADER ? vertexShaderLog() : fragmentShaderLog();
		if (logBytes)
		{
			Log("cocos2d: %s", logBytes);
			free(logBytes);
		}

		abort();
	}
	return (status == GL_TRUE);
}


bool GLProgram::link()
{
	GLint status = GL_TRUE;

	glLinkProgram(_uProgram);

	if (_uVertShader)
	{
		glDeleteShader(_uVertShader);
	}

	if (_uFragShader)
	{
		glDeleteShader(_uFragShader);
	}

	_uVertShader = _uFragShader = 0;
	glGetProgramiv(_uProgram, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		char* logBytes = programLog();
		if (logBytes)
		{
			Log("cocos2d: ERROR: Failed to link program: %i\n%s", logBytes);
			free(logBytes);
		}
		
		glDeleteProgram(_uProgram);
		_uProgram = 0;
	}

	return (status == GL_TRUE);
}

void GLProgram::use()
{
	// debug下调试用  会影响性能
	//glValidateProgram(program);
	//CheckProgram(_uProgram, GL_VALIDATE_STATUS);

	glUseProgram(_uProgram);
}


char* GLProgram::programLog()
{
	return this->logForOpenGLObject(_uProgram, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

char* GLProgram::vertexShaderLog()
{
	return this->logForOpenGLObject(_uVertShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

char* GLProgram::fragmentShaderLog()
{
	return this->logForOpenGLObject(_uFragShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

char* GLProgram::logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc)
{
	GLint logLength = 0, charsWritten = 0;

	infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength < 1)
		return 0;

	char* logBytes = (char*)malloc(logLength);
	logFunc(object, logLength, &charsWritten, logBytes);

	return logBytes;
}


void GLProgram::addAttribute(const char* attributeName, GLuint index)
{
	glBindAttribLocation(_uProgram, index, attributeName);
}


void GLProgram::updateUniforms()
{
	_uUniforms[kCCUniformPMatrix]   = glGetUniformLocation(_uProgram, kCCUniformPMatrix_s);
	_uUniforms[kCCUniformMVMatrix]  = glGetUniformLocation(_uProgram, kCCUniformMVMatrix_s);
	_uUniforms[kCCUniformMVPMatrix] = glGetUniformLocation(_uProgram, kCCUniformMVPMatrix_s);

	_uUniforms[kCCUniformTime]    = glGetUniformLocation(_uProgram, kCCUniformTime_s);
	_uUniforms[kCCUniformSinTime] = glGetUniformLocation(_uProgram, kCCUniformSinTime_s);
	_uUniforms[kCCUniformCosTime] = glGetUniformLocation(_uProgram, kCCUniformCosTime_s);

	_uUniforms[kCCUniformRandom01] = glGetUniformLocation(_uProgram, kCCUniformRandom01_s);
	_uUniforms[kCCUniformSampler]  = glGetUniformLocation(_uProgram, kCCUniformSampler_s);

	this->use();

	// Since sample most probably won't change, set it to 0 now.
	this->setUniformLocationWith1i(_uUniforms[kCCUniformSampler], 0);
}


GLint GLProgram::getUniformLocationForName(const char* name)
{
	GLuint location = glGetUniformLocation(_uProgram, name);
	if (0xffffffff == location)
	{
		Log("glGetUniformLocation failed");
	}
	return location;
}


void GLProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
	bool updated = updateUniformLocation(location, &i1, sizeof(i1) * 1);

	if (updated)
	{
		glUniform1i((GLint)location, i1);
	}
}

void GLProgram::setUniformLocationWith2i(GLint location, GLint i1, GLint i2)
{
	GLint ints[2] = { i1,i2 };
	bool updated = updateUniformLocation(location, ints, sizeof(ints));

	if (updated)
	{
		glUniform2i((GLint)location, i1, i2);
	}
}

void GLProgram::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3)
{
	GLint ints[3] = { i1,i2,i3 };
	bool updated = updateUniformLocation(location, ints, sizeof(ints));

	if (updated)
	{
		glUniform3i((GLint)location, i1, i2, i3);
	}
}

void GLProgram::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4)
{
	GLint ints[4] = { i1,i2,i3,i4 };
	bool updated = updateUniformLocation(location, ints, sizeof(ints));

	if (updated)
	{
		glUniform4i((GLint)location, i1, i2, i3, i4);
	}
}



void GLProgram::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
	bool updated = updateUniformLocation(location, ints, sizeof(int) * 2 * numberOfArrays);

	if (updated)
	{
		glUniform2iv((GLint)location, (GLsizei)numberOfArrays, ints);
	}
}

void GLProgram::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
	bool updated = updateUniformLocation(location, ints, sizeof(int) * 3 * numberOfArrays);

	if (updated)
	{
		glUniform3iv((GLint)location, (GLsizei)numberOfArrays, ints);
	}
}

void GLProgram::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
	bool updated = updateUniformLocation(location, ints, sizeof(int) * 4 * numberOfArrays);

	if (updated)
	{
		glUniform4iv((GLint)location, (GLsizei)numberOfArrays, ints);
	}
}


void GLProgram::setUniformLocationWith1f(GLint location, GLfloat f1)
{
	bool updated = updateUniformLocation(location, &f1, sizeof(f1) * 1);

	if (updated)
	{
		glUniform1f((GLint)location, f1);
	}
}

void GLProgram::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2)
{
	GLfloat floats[2] = { f1,f2 };
	bool updated = updateUniformLocation(location, floats, sizeof(floats));

	if (updated)
	{
		glUniform2f((GLint)location, f1, f2);
	}
}

void GLProgram::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3)
{
	GLfloat floats[3] = { f1,f2,f3 };
	bool updated = updateUniformLocation(location, floats, sizeof(floats));

	if (updated)
	{
		glUniform3f((GLint)location, f1, f2, f3);
	}
}

void GLProgram::setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
	GLfloat floats[4] = { f1,f2,f3,f4 };
	bool updated = updateUniformLocation(location, floats, sizeof(floats));

	if (updated)
	{
		glUniform4f((GLint)location, f1, f2, f3, f4);
	}
}


void GLProgram::setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
	bool updated = updateUniformLocation(location, floats, sizeof(float) * 2 * numberOfArrays);

	if (updated)
	{
		glUniform2fv((GLint)location, (GLsizei)numberOfArrays, floats);
	}
}

void GLProgram::setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
	bool updated = updateUniformLocation(location, floats, sizeof(float) * 3 * numberOfArrays);

	if (updated)
	{
		glUniform3fv((GLint)location, (GLsizei)numberOfArrays, floats);
	}
}

void GLProgram::setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
	bool updated = updateUniformLocation(location, floats, sizeof(float) * 4 * numberOfArrays);

	if (updated)
	{
		glUniform4fv((GLint)location, (GLsizei)numberOfArrays, floats);
	}
}


void GLProgram::setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices)
{
	bool updated = updateUniformLocation(location, matrixArray, sizeof(float) * 16 * numberOfMatrices);

	if (updated)
	{
		glUniformMatrix4fv((GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
	}
}


bool GLProgram::updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes)
{
	//本来缓存location上的数据  若和上次一致 就不用上传服务器了
	return true;
}