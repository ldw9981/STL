#pragma once

#include <Windows.h>

#include <d3d11.h>
#include <d3dx11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

class DXApp
{
public:
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

protected:

	// �ʱ�ȭ �Լ�(�޼ҵ�).
	bool InitWindow();

	// DirectX �ʱ�ȭ �Լ�.
	bool InitDirect3D();
};