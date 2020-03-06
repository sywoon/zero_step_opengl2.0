#include "Math3d.h"
#include <math.h>


Vector3f::Vector3f()
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
{
}

Vector3f::Vector3f(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}

Vector3f Vector3f::Cross(const Vector3f & v) const
{
	const float xx = y * v.z - z * v.y;
	const float yy = z*v.x - x*v.z;
	const float zz = x*v.y - y*v.x;

	return Vector3f(xx, yy, zz);
}

Vector3f & Vector3f::Normalize()
{
	const float length = sqrtf(x*x + y*y + z*z);
	x /= length;
	y /= length;
	z /= length;

	return *this;
}


