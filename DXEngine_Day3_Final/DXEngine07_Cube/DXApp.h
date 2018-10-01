#pragma once

#include <Windows.h>
#include "DXUtil.h"

class DXApp
{
public:
	// ���� ����.
	struct Vertex
	{
		XMFLOAT3 position;		// ���� ��ġ ����.
		//XMFLOAT4 color;			// ���� ���� ����.
		XMFLOAT2 texCoord;		// �ؽ�ó ��ǥ(UV).
		XMFLOAT3 normal;		// ��� ����.

		Vertex() { }
		Vertex(float x, float y, float z) : position(x, y, z) { }
		Vertex(XMFLOAT3 position) : position(position) { }

		Vertex(XMFLOAT3 position, 
			XMFLOAT2 texCoord, 
			XMFLOAT3 normal) : position(position), texCoord(texCoord),
			normal(normal) { }
	};

	// ��� ���ۿ� ����ü.
	struct CBPerObject
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	int Run();

	virtual bool Init();
	virtual void Update() = 0;
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

	ID3D11Buffer* vertexBuffer;		// ���� ����.
	ID3D11VertexShader* vertexShader;		// ���� ���̴�.
	ID3D11PixelShader* pixelShader;			// �ȼ� ���̴�.
	ID3DBlob* vertexShaderBuffer;		// ���� ���̴� ����.
	ID3DBlob* pixelShaderBuffer;		// �ȼ� ���̴� ����.
	ID3D11InputLayout* vertexInputLayout;	// �Է� ���̾ƿ�.

	int nVertices;							// ���� ����.
	Vertex* vertices;						// ���� �迭.

	ID3D11Buffer* indexBuffer;			// �ε���(����) ����.
	int nIndices;								// �ε��� ����.
	DWORD* indices;						// �ε��� �迭.

	// ������ȯ�� �ʿ��� ���� ����.
	XMMATRIX worldMatrix;				// ���� ��ȯ ���.
	XMMATRIX viewMatrix;				// �� ��ȯ ���.
	XMMATRIX projectionMatrix;		// ���� ��ȯ ���.

	XMVECTOR cameraPos;				// ī�޶� ��ġ ����.
	XMVECTOR cameraTarget;			// ī�޶� �ٶ� ���� ����.
	XMVECTOR cameraUp;				// ī�޶� �� ���� ����.

	ID3D11Buffer* cBuffer;				// �������.

	// �ؽ�ó / ���÷� ������Ʈ.
	ID3D11ShaderResourceView* pTexture;
	ID3D11SamplerState* pSamplerState;

protected:

	// �ʱ�ȭ �Լ�(�޼ҵ�).
	bool InitWindow();

	// DirectX �ʱ�ȭ �Լ�.
	bool InitDirect3D();

	// DirectX ��� �ʱ�ȭ.
	bool InitScene();

	// �� ���� �ʱ�ȭ (ť��).
	bool LoadModel(const char* fileName);

	// ���� ��ȯ ��� �ʱ�ȭ.
	bool InitTransformation();

	// �ؽ�ó �ʱ�ȭ.
	bool InitTexture();
};