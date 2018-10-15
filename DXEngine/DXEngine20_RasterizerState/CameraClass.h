#pragma once

#include "DXUtil.h"

class CameraClass
{
public:
	CameraClass();
	~CameraClass();

	float GetFovY() const { return fovY * Rad2Deg; }
	void SetFovY(float fovY);

	float GetAspecRation() const { return aspectRatio; }
	void SetAspecRation(UINT width, UINT height);

	void GetPlaneDistance(float& zN, float& zF);
	void SetPlaneDistance(float zN, float zF);

	XMMATRIX GetViewMatrix() const { return viewMatrix; }
	XMMATRIX GetProjectionMatrix() const { return projectionMatrix; }

	XMVECTOR GetPosition() const { return cameraPosition; }
	void SetPosition(XMVECTOR pos);

	XMVECTOR GetRight() const { return cameraRight; }
	XMVECTOR GetLook() const { return cameraLook; }
	XMVECTOR GetUp() const { return cameraUp; }

	void MoveForward(float dist);
	void MoveRight(float dist);
	void MoveUp(float dist);

	void Yaw(float angle);
	void Pitch(float angle);

	void UpdateCamera();

private:

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:

	float fovY;				//�þ߰�.
	float aspectRatio;		// ȭ�� ��Ⱦ��.
	float nearZ;				// ����� �Ÿ�.
	float farZ;				// ����� �Ÿ�.

	float yaw = 0.0f;		// yaw ȸ�� ��.
	float pitch = 0.0f;		// pitch ȸ�� ��.

	float moveSpeed = 200.0f;		// ī�޶� �̵� �ӵ�.
	float rotationSpeed = 30.0f;		// ī�޶� ȸ�� �ӵ�.

	bool modifiedProjection = false;

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMVECTOR cameraPosition;
	XMVECTOR cameraLook 
		= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR cameraRight 
		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR cameraForward
		= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMVECTOR defaultRight 
		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR defaultUp
		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR defaultForward
		= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
};