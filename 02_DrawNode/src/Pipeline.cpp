#include "Pipeline.h"
#include <math.h>



Pipeline::Pipeline()
{
	Reset();	
}


Pipeline::~Pipeline()
{
}

void Pipeline::Reset()
{
	_scale = Vector3f(1.0f, 1.0f, 1.0f);
	_rotation = Vector3f(0.0f, 0.0f, 0.0f);
	_translate = Vector3f(0.0f, 0.0f, 0.0f);
}

void Pipeline::Scale(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
}

void Pipeline::Rotate(float x, float y, float z)
{
	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;
}

void Pipeline::Translate(float x, float y, float z)
{
	_translate.x = x;
	_translate.y = y;
	_translate.z = z;
}

void Pipeline::SetPerspectiveProjection(float fov, float width, float height, float zNear, float zFar)
{
	_persProj.fov = fov;
	_persProj.width = width;
	_persProj.height = height;
	_persProj.zNear = zNear;
	_persProj.zFar = zFar;
}

void Pipeline::SetCamera(const Vector3f & cameraPos, const Vector3f & cameraTarget, const Vector3f & cameraUp)
{
	_camera.pos = cameraPos;
	_camera.target = cameraTarget;
	_camera.up = cameraUp;
}

void Pipeline::InitScaleMat(Matrix4f& mat)
{
	mat.mat[0][0] = _scale.x;
	mat.mat[0][1] = 0.0f;
	mat.mat[0][2] = 0.0f;
	mat.mat[0][3] = 0.0f;

	mat.mat[1][0] = 0.0f;
	mat.mat[1][1] = _scale.y;
	mat.mat[1][2] = 0.0f;
	mat.mat[1][3] = 0.0f;

	mat.mat[2][0] = 0.0f;
	mat.mat[2][1] = 0.0f;
	mat.mat[2][2] = _scale.z;
	mat.mat[2][3] = 0.0f;

	mat.mat[3][0] = 0.0f;
	mat.mat[3][1] = 0.0f;
	mat.mat[3][2] = 0.0f;
	mat.mat[3][3] = 1.0f;
}

void Pipeline::InitRotateMat(Matrix4f& mat)
{
	Matrix4f rx, ry, rz;

	const float x = ToRadian(_rotation.x);
	const float y = ToRadian(_rotation.y);
	const float z = ToRadian(_rotation.z);

	rx.mat[0][0] = 1.0f; rx.mat[0][1] = 0.0f; rx.mat[0][2] = 0.0f; rx.mat[0][3] = 0.0f;
	rx.mat[1][0] = 0.0f; rx.mat[1][1] = cosf(x); rx.mat[1][2] = -sinf(x); rx.mat[1][3] = 0.0f;
	rx.mat[2][0] = 0.0f; rx.mat[2][1] = sinf(x); rx.mat[2][2] = cosf(x); rx.mat[2][3] = 0.0f;
	rx.mat[3][0] = 0.0f; rx.mat[3][1] = 0.0f; rx.mat[3][2] = 0.0f; rx.mat[3][3] = 1.0f;

	ry.mat[0][0] = cosf(y); ry.mat[0][1] = 0.0f; ry.mat[0][2] = -sinf(y); ry.mat[0][3] = 0.0f;
	ry.mat[1][0] = 0.0f; ry.mat[1][1] = 1.0f; ry.mat[1][2] = 0.0f; ry.mat[1][3] = 0.0f;
	ry.mat[2][0] = sinf(y); ry.mat[2][1] = 0.0f; ry.mat[2][2] = cosf(y); ry.mat[2][3] = 0.0f;
	ry.mat[3][0] = 0.0f; ry.mat[3][1] = 0.0f; ry.mat[3][2] = 0.0f; ry.mat[3][3] = 1.0f;

	rz.mat[0][0] = cosf(x); rz.mat[0][1] = -sinf(x); rz.mat[0][2] = 0.0f; rz.mat[0][3] = 0.0f;
	rz.mat[1][0] = sinf(x); rz.mat[1][1] = cosf(x); rz.mat[1][2] = 0.0f;; rz.mat[1][3] = 0.0f;
	rz.mat[2][0] = 0.0f; rz.mat[2][1] = 0.0f; rz.mat[2][2] = 1.0f; rz.mat[2][3] = 0.0f;
	rz.mat[3][0] = 0.0f; rz.mat[3][1] = 0.0f; rz.mat[3][2] = 0.0f; rz.mat[3][3] = 1.0f;

	mat = rz * ry * rx;
}

void Pipeline::InitTransformMat(Matrix4f& mat)
{
	mat.mat[0][0] = 1.0f;
	mat.mat[0][1] = 0.0f;
	mat.mat[0][2] = 0.0f;
	mat.mat[0][3] = _translate.x;

	mat.mat[1][0] = 0.0f;
	mat.mat[1][1] = 1.0f;
	mat.mat[1][2] = 0.0f;
	mat.mat[1][3] = _translate.y;

	mat.mat[2][0] = 0.0f;
	mat.mat[2][1] = 0.0f;
	mat.mat[2][2] = 1.0f;
	mat.mat[2][3] = _translate.z;

	mat.mat[3][0] = 0.0f;
	mat.mat[3][1] = 0.0f;
	mat.mat[3][2] = 0.0f;
	mat.mat[3][3] = 1.0f;
}

void Pipeline::InitPerspectiveProjection(Matrix4f & mat)
{
	const float ar = _persProj.width / _persProj.height;
	const float zRange = _persProj.zNear - _persProj.zFar;
	const float tanHalfFOV = tanf(ToRadian(_persProj.fov / 2.0f));

	mat.mat[0][0] = 1.0f / (tanHalfFOV * ar);
	mat.mat[0][1] = 0.0f;
	mat.mat[0][2] = 0.0f;
	mat.mat[0][3] = 0.0;

	mat.mat[1][0] = 0.0f;
	mat.mat[1][1] = 1.0f / tanHalfFOV;
	mat.mat[1][2] = 0.0f;
	mat.mat[1][3] = 0.0;

	mat.mat[2][0] = 0.0f;
	mat.mat[2][1] = 0.0f;
	mat.mat[2][2] = (-_persProj.zNear - _persProj.zFar) / zRange;
	mat.mat[2][3] = 2.0f*_persProj.zFar*_persProj.zNear / zRange;

	mat.mat[3][0] = 0.0f;
	mat.mat[3][1] = 0.0f;
	mat.mat[3][2] = 1.0f;
	mat.mat[3][3] = 0.0;


	//const float ar = _persProj.width / _persProj.height;
	//const float zNear = -_persProj.zNear;
	//const float zFar = -_persProj.zFar;
	//const float zRange = zNear - zFar;
	//const float tanHalfFov = tanf(ToRadian(_persProj.fov / 2.0f));

	//mat.mat[0][0] = 1.0f/(tanHalfFov * ar);
	//mat.mat[0][1] = 0.0f;
	//mat.mat[0][2] = 0.0f;
	//mat.mat[0][3] = 0.0f;

	//mat.mat[1][0] = 0.0f;
	//mat.mat[1][1] = 1.0f/tanHalfFov;
	//mat.mat[1][2] = 0.0f;
	//mat.mat[1][3] = 0.0f;

	//mat.mat[2][0] = 0.0f;
	//mat.mat[2][1] = 0.0f;
	//mat.mat[2][2] = (-zNear-zFar)/zRange;
	//mat.mat[2][3] = 2.0f*zFar*zNear/zRange;

	//mat.mat[3][0] = 0.0f;
	//mat.mat[3][1] = 0.0f;
	//mat.mat[3][2] = -1.0f;
	//mat.mat[3][3] = 0.0f;
}

void Pipeline::InitCameraMat(Matrix4f & matTrans, Matrix4f & matRot)
{
	matTrans.mat[0][0] = 1.0f; matTrans.mat[0][1] = 0.0f;
	matTrans.mat[0][2] = 0.0f; matTrans.mat[0][3] = -_camera.pos.x;

	matTrans.mat[1][0] = 0.0f; matTrans.mat[1][1] = 1.0f;
	matTrans.mat[1][2] = 0.0f; matTrans.mat[1][3] = -_camera.pos.y;

	matTrans.mat[2][0] = 0.0f; matTrans.mat[2][1] = 0.0f;
	matTrans.mat[2][2] = 1.0f; matTrans.mat[2][3] = -_camera.pos.z;

	matTrans.mat[3][0] = 0.0f; matTrans.mat[3][1] = 0.0f;
	matTrans.mat[3][2] = 0.0f; matTrans.mat[3][3] = 1.0f;

	Vector3f N = _camera.target;
	N.Normalize();
	Vector3f U = _camera.up;
	U.Normalize();
	U = U.Cross(N);
	Vector3f V = N.Cross(U);

	matRot.mat[0][0] = U.x; matRot.mat[0][1] = U.y;
	matRot.mat[0][2] = U.z; matRot.mat[0][3] = 0;

	matRot.mat[1][0] = V.x; matRot.mat[1][1] = V.y;
	matRot.mat[1][2] = V.z; matRot.mat[1][3] = 0;

	matRot.mat[2][0] = N.x; matRot.mat[2][1] = N.y;
	matRot.mat[2][2] = N.z; matRot.mat[2][3] = 0;

	matRot.mat[3][0] = 0.0f; matRot.mat[3][1] = 0.0f;
	matRot.mat[3][2] = 0.0f; matRot.mat[3][3] = 1.0f;
}


const Matrix4f* Pipeline::GetMat()
{
	Matrix4f matScale, matRoate, matTrans, matPersProj;
	Matrix4f matCameraTrans, matCameraRot;

	InitScaleMat(matScale);
	InitRotateMat(matRoate);
	InitTransformMat(matTrans);
	InitPerspectiveProjection(matPersProj);
	InitCameraMat(matCameraTrans, matCameraRot);

	//列主序：从右到左
	Matrix4f matM = matTrans * matRoate * matScale;
	Matrix4f matV = matCameraRot * matCameraTrans;  // (A*B)-1 = B-1 *  A-1 (-1表示逆)?
	_mat = matPersProj * matV * matM;
	return &_mat;
}
