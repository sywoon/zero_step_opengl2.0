#pragma once
#include "GL/glew.h"
#include <math.h>


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




static ccVertex2F v2fzero = { 0.0f,0.0f };

static inline ccVertex2F v2f(float x, float y)
{
	ccVertex2F ret = { x, y };
	return ret;
}

static inline ccVertex2F v2fadd(const ccVertex2F& v0, const ccVertex2F& v1)
{
	return v2f(v0.x + v1.x, v0.y + v1.y);
}

static inline ccVertex2F v2fsub(const ccVertex2F& v0, const ccVertex2F& v1)
{
	return v2f(v0.x - v1.x, v0.y - v1.y);
}

static inline ccVertex2F v2fmult(const ccVertex2F& v, float s)
{
	return v2f(v.x * s, v.y * s);
}

static inline ccVertex2F v2fperp(const ccVertex2F& p0)
{
	return v2f(-p0.y, p0.x);
}

static inline ccVertex2F v2fneg(const ccVertex2F& p0)
{
	return v2f(-p0.x, -p0.y);
}

static inline float v2fdot(const ccVertex2F& p0, const ccVertex2F& p1)
{
	return  p0.x * p1.x + p0.y * p1.y;
}

static inline ccVertex2F v2fforangle(float _a_)
{
	return v2f(cosf(_a_), sinf(_a_));
}

static inline ccVertex2F v2fnormalize(const ccVertex2F& p)
{
	float x = p.x;
	float y = p.y;
	float length = sqrtf(x * x + y * y);
	if (length == 0.) return ccVertex2F{ 1.f, 0 };

	return ccVertex2F{x/length, y/length};
}

static inline ccTex2F __t(const ccVertex2F& v)
{
	return *(ccTex2F*)&v;
}



