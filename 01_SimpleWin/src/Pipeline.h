#pragma once
#include "Math3d.h"


class Pipeline
{
	struct PerspectiveProjection{
		float fov;
		float width;
		float height;
		float zNear;
		float zFar;
	};

	struct Camera
	{
		Vector3f pos;
		Vector3f target;
		Vector3f up;
	};

public:
	Pipeline();
	~Pipeline();

	void Reset();
	const Matrix4f* GetMat();
	void Scale(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Translate(float x, float y, float z);
	void SetPerspectiveProjection(float fov, float width, float height, float zNear, float zFar);
	void SetCamera(const Vector3f& cameraPos, const Vector3f&cameraTarget, const Vector3f&cameraUp);

private:
	void InitScaleMat(Matrix4f& mat);
	void InitRotateMat(Matrix4f& mat);
	void InitTransformMat(Matrix4f& mat);
	void InitPerspectiveProjection(Matrix4f& mat);
	void InitCameraMat(Matrix4f & matTrans, Matrix4f & matRot);

private:
	Vector3f _scale;
	Vector3f _rotation;
	Vector3f _translate;

	Matrix4f _mat;

	PerspectiveProjection _persProj;
	Camera _camera;
};

