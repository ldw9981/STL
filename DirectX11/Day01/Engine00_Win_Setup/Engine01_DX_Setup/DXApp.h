#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>

#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"d3dx11.lib")

class DXApp
{
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

protected:

	bool InitWindow();
	bool InitDirect3D();

};

