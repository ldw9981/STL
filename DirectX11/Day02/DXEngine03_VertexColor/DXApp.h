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
		XMFLOAT4 color;			// ���� ���� ����.

		Vertex(float x, float y, float z) : position(x, y, z) { }
		Vertex(XMFLOAT3 position) : position(position) { }
		
		Vertex(XMFLOAT3 position, XMFLOAT4 color) 
			: position(position), color(color) { }
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

protected:

	// �ʱ�ȭ �Լ�(�޼ҵ�).
	bool InitWindow();

	// DirectX �ʱ�ȭ �Լ�.
	bool InitDirect3D();

	// DirectX ��� �ʱ�ȭ.
	bool InitScene();
};