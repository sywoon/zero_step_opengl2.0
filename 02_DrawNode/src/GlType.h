#pragma once
#include "GL/glew.h"


#ifndef MIN
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif

#ifndef MAX
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif

enum {
	kCCVertexAttrib_Position,
	kCCVertexAttrib_Color,
	kCCVertexAttrib_TexCoords,

	kCCVertexAttrib_MAX,
};



typedef struct _ccVertex2F
{
	GLfloat x;
	GLfloat y;
} ccVertex2F;

typedef struct _ccColor4B
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
} ccColor4B;

typedef struct _ccTex2F {
	GLfloat u;
	GLfloat v;
} ccTex2F;




typedef struct _ccV2F_C4B_T2F
{
    //! vertices (2F)
    ccVertex2F        vertices;
    //! colors (4B)
    ccColor4B         colors;
    //! tex coords (2F)
    ccTex2F           texCoords;
} ccV2F_C4B_T2F;



typedef struct _ccV2F_C4B_T2F_Triangle
{
	//! Point A
	ccV2F_C4B_T2F a;
	//! Point B
	ccV2F_C4B_T2F b;
	//! Point B
	ccV2F_C4B_T2F c;
} ccV2F_C4B_T2F_Triangle;








