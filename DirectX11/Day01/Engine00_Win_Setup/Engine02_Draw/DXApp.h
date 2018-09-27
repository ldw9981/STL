#pragma once
#include <Windows.h>

#include "DXUtil.h"


class DXApp
{
public:
	struct Vertex
	{
		float x;
		float y;
		float z;
		Vertex(float InitX, float InitY, float InitZ) :x(InitX), y(InitY), z(InitZ)
		{

		}
	};

public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();

	int Run();

	virtual bool Init();
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual LRESULT MSGProc(HWND, UINT, WPARAM, LPARAM);

protected:
	// windows
	HWND hwnd;
	HINSTANCE hinstance;
	UINT clientWidth;
	UINT clientHeight;
	LPCWSTR applicationName;
	DWORD wndStyle;

	//dx
	ID3D11Device* pDevice;				
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;

	ID3D11Buffer* pVertexBuffer;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3DBlob* pVertexShaderBuffer;
	ID3DBlob* pPixelShaderBuffer;
	ID3D11InputLayout* pVertextInputLayout; // 입력 레이 아웃

protected:

	bool InitWindow();
	bool InitDirect3D();
	bool InitScene();
};

