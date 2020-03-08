#pragma once

#define M_PI 3.14159265
#define ToRadian(x) ( (float)((x)*M_PI / 180.0f) )
#define ToDegree(x) ( (float)((x)*180.0f / M_PI) )



class Vector3f
{
public:
	Vector3f();
	Vector3f(float xx, float yy, float zz);
	Vector3f Cross(const Vector3f& v) const;
	Vector3f& Normalize();

public:
	float x, y, z;
};

class Matrix4f
{
public:
	inline Matrix4f operator* (const Matrix4f& other)
	{
		Matrix4f rtn;
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				rtn.mat[i][j] = mat[i][0] * other.mat[0][j] +
								mat[i][1] * other.mat[1][j] +
								mat[i][2] * other.mat[2][j] +
								mat[i][3] * other.mat[3][j];
			}
		}
		return rtn;
	}

public:
	float mat[4][4];
};