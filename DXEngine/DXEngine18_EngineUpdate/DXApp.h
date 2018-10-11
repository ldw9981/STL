#pragma once

#include <Windows.h>
#include "DXUtil.h"
#include "GameTimer.h"
#include <vector>
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "FBXLoader.h"

class DXApp
{
public:
	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	int Run();

	virtual bool Init();
	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;
	virtual LRESULT MSGProc(HWND, UINT, WPARAM, LPARAM);

protected:

	// Win32 ���� ����.
	HWND hwnd;
	HINSTANCE hinstance;
	UINT clientWidth;
	UINT clientHeight;
	LPCWSTR applicationName;
	DWORD wndStyle;

	// DirectX ����.
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;

	// ����/���ٽ� ����.
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	// ������ȯ�� �ʿ��� ���� ����.
	XMMATRIX worldMatrix;				// ���� ��ȯ ���.
	XMMATRIX viewMatrix;				// �� ��ȯ ���.
	XMMATRIX projectionMatrix;		// ���� ��ȯ ���.

	XMVECTOR cameraPos;				// ī�޶� ��ġ ����.
	XMVECTOR cameraTarget;			// ī�޶� �ٶ� ���� ����.
	XMVECTOR cameraUp;				// ī�޶� �� ���� ����.

	ID3D11Buffer* cBuffer;				// ������� - WVP.
	ID3D11Buffer* lightBuffer;		// ������� - ����Ʈ��.
	GameTimer gameTimer;			// ���� Ÿ�̸�.

	// �޽�.
	std::vector<Mesh> meshes;

protected:

	// �ʱ�ȭ �Լ�(�޼ҵ�).
	bool InitWindow();

	// DirectX �ʱ�ȭ �Լ�.
	bool InitDirect3D();

	// ���� ���� �ʱ�ȭ.
	bool InitDepthStencilBuffer();

	// DirectX ��� �ʱ�ȭ.
	bool InitScene();

	// �޽� �ʱ�ȭ.
	void InitMeshInfo();

	// ������/���ε� ���̴�.
	bool CompileShader(Mesh* mesh);
	void BindShader(Mesh* mesh);

	// ���� ���� ����.
	bool InitVertexBuffer(Mesh* mesh);
	void BindVertexBuffer(Mesh* mesh);

	// �ε��� ���� ����.
	bool InitIndexBuffer(Mesh* mesh);
	void BindIndexBuffer(Mesh* mesh);

	// �Է� ���̾ƿ� ����.
	bool InitInputLayout(Mesh* mesh);
	void BindInputLayout(Mesh* mesh);

	// ���� ��ȯ ��� �ʱ�ȭ.
	bool InitTransformation();
	bool InitTransformation(Mesh* mesh);

	void InitWorldMatrix(Mesh* mesh);
	void InitViewMatrix();
	void InitProjectionMatrix();

	bool InitWVPBuffer(Mesh* mesh);
	void UpdateWVPBuffer(Mesh* mesh);

	// ����Ʈ ���� �ʱ�ȭ.
	bool InitLightCB();

	// ����Ʈ ���� ����.
	void UpdateLightCB(Mesh* mesh);

	// ����Ʈ ���� ���ε�.
	void BindLightCB();

	// ����Ʈ ����.
	void SetViewport();
};