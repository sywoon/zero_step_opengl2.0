#pragma once


#include "GL/glew.h"
#include "Log.h"


typedef void (*GLInfoFunction)(GLuint program, GLenum pname, GLint* params);
typedef void (*GLLogFunction) (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog);

#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            Log("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)



enum {
	kCCUniformPMatrix,
	kCCUniformMVMatrix,
	kCCUniformMVPMatrix,

	kCCUniformTime,
	kCCUniformSinTime,
	kCCUniformCosTime,

	kCCUniformRandom01,
	kCCUniformSampler,

	kCCUniform_MAX,
};

// uniform names  shaderÖÐµÄÃû³Æ
#define kCCUniformPMatrix_s				"CC_PMatrix"
#define kCCUniformMVMatrix_s			"CC_MVMatrix"
#define kCCUniformMVPMatrix_s			"CC_MVPMatrix"
#define kCCUniformTime_s				"CC_Time"
#define kCCUniformSinTime_s				"CC_SinTime"
#define kCCUniformCosTime_s				"CC_CosTime"
#define kCCUniformRandom01_s			"CC_Random01"
#define kCCUniformSampler_s				"CC_Texture0"
#define kCCUniformAlphaTestValue		"CC_alpha_value"


class GLProgram
{
public:
	GLProgram();
	~GLProgram();

public:
	bool initWithVertexShaderFilename(const char* vShaderFilename, const char* fShaderFilename);
	bool initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);

	bool link();
    void use();

    void addAttribute(const char* attributeName, GLuint index);
    void updateUniforms();

    GLint getUniformLocationForName(const char* name);
    void setUniformLocationWith1i(GLint location, GLint i1);
    void setUniformLocationWith2i(GLint location, GLint i1, GLint i2);
    void setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3);
    void setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4);

    void setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays);
    void setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays);
    void setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays);

    void setUniformLocationWith1f(GLint location, GLfloat f1);
    void setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2);
    void setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3);
    void setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);

    void setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);
    void setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);
    void setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);
    void setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices);


private:
    bool        compileShader(GLuint* shader, GLenum type, const GLchar* source);
    bool        updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes);

    char*       programLog();
    char*       vertexShaderLog();
    char*       fragmentShaderLog();
    char*       logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc);


private:
	GLuint            _uProgram;
	GLuint            _uVertShader;
	GLuint            _uFragShader;

	GLint             _uUniforms[kCCUniform_MAX];
};


