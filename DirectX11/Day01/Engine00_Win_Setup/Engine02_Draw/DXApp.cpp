#pragma once
#include "DXApp.h"

DXApp* pApp = NULL;

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (pApp != NULL)
		pApp->MSGProc(hWnd, msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hInstance)
{
	// window
	pApp = this;
	hwnd = NULL;
	hinstance = hInstance;
	clientWidth = 800;
	clientHeight = 600;
	applicationName = L"Engine01 DX Setup";
	wndStyle = WS_OVERLAPPEDWINDOW;

	// dx
	pDevice=NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;
}

DXApp::~DXApp()
{
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pSwapChain);
	Memory::SafeRelease(pRenderTargetView);

	Memory::SafeRelease(pVertexBuffer);
	Memory::SafeRelease(pVertexShader);
	Memory::SafeRelease(pPixelShader);
	Memory::SafeRelease(pVertexShaderBuffer);
	Memory::SafeRelease(pixelShaderBuffer);
	Memory::SafeRelease(vertextInputLayout);
}

int DXApp::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 루프 실행.
	while (msg.message != WM_QUIT)
	{
		// 메시지 매핑.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 엔진 업데이트.
			Update();

			// 화면 그리기.
			Render();
		}
	}

	/* 
	// 뭐가 다른거지
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			Update();
			Render();
		}
		/*
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		
	}
	*/

	/*
		// 메시지 처리 루프.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 루프 실행.
	while (msg.message != WM_QUIT)
	{
		// 메시지 매핑.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 엔진 업데이트.
			Update();

			// 화면 그리기.
			Render();
		}
	}

	*/
	return 0;
}

bool DXApp::Init()
{
	if(InitWindow() == false)	
		return false;

	if (InitDirect3D() == false)
		return false;

	return true;
}

LRESULT DXApp::MSGProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

bool DXApp::InitWindow()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(wc);
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = NULL;
	wc.lpszClassName = L"WindowsClass";
	wc.lpfnWndProc = WinProc;

	if (!RegisterClassEx(&wc))
		return false;

	//HWND hWnd;
	hwnd = CreateWindow(L"WindowsClass", this->applicationName, wndStyle, 0, 0, clientWidth,clientHeight, NULL, NULL, hinstance, NULL);

	if (hwnd == NULL)
		return false;

	ShowWindow(hwnd, SW_SHOW);

	return true;
}

bool DXApp::InitDirect3D()
{
	HRESULT  hr;

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
	//백버퍼 설정
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.Width = clientWidth;
	swapDesc.BufferDesc.Height = clientHeight;	
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;  	// 쿼리후 설정해야하지만 ,화면 주사율 임의설정
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;	// 쿼리후 설정해야하지만 ,화면 주사율 임의설정


	swapDesc.SampleDesc.Count = 1;		// 쿼리후 설정해야하지만 ,샘플링 사용안함으로 임의설정 
	swapDesc.SampleDesc.Quality = 0;

	hr = D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,NULL,NULL,NULL,
		D3D11_SDK_VERSION,&swapDesc,&pSwapChain,&pDevice,NULL,&pDeviceContext);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"장치 생성 실패.", L"오류", MB_OK);
	}

	ID3D11Texture2D* pBackBufferTxexture;
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTxexture); // 백버퍼 포인터 얻기
	if (FAILED(hr))
	{
		MessageBox(NULL, L"백버퍼 생성 실패.", L"오류", MB_OK);
	}

	hr = pDevice->CreateRenderTargetView(pBackBufferTxexture, NULL, &pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"렌더 타겟 생성 실패.", L"오류", MB_OK);
	}

	//
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	if (pBackBufferTxexture)
	{
		pBackBufferTxexture->Release();
		pBackBufferTxexture = NULL;
	}


	return true;
}

bool DXApp::InitScene()
{
	HRESULT hr;
	//////////////////
	//정점 쉐이더 처리
	hr = D3DX11CompileFromFile(L"EffectVS.fx", NULL, NULL, L"main", "vs_4_0", NULL, NULL, NULL, &pVertexShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"정점 쉐이더 컴파일 실패.", L"오류", MB_OK);
		return false;
	}
	hr = pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"정점 쉐이더 생성 실패.", L"오류", MB_OK);
		return false;
	}
	// 바인딩
	pDeviceContext->VSSetShader(pVertexShader, NULL, NULL);

	//////////////////
	//픽셀 쉐이더 처리
	hr = D3DX11CompileFromFile(L"EffectPS.fx", NULL, NULL, L"main", "ps_4_0", NULL, NULL, NULL, &pPixelShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"픽셀 쉐이더 컴파일 실패.", L"오류", MB_OK);
		return false;
	}
	hr = pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(),
		pPixelShaderBuffer->GetBufferSize(), NULL, &pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"픽셀 쉐이더 생성 실패.", L"오류", MB_OK);
		return false;
	}
	// 바인딩
	pDeviceContext->PSSetShader(pPixelShader, NULL, NULL);


	// 왼손좌표계 기준의 정점이다. 
	Vertex vertices[] =
	{
		Vertex(0.0f,0.5f,0.5f),
		Vertex(0.0f,-0.5f,0.5f),
		Vertex(-0.5f,-0.5f,0.5f),
	};

	// 버텍스 버퍼
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	pVertexBuffer = pDeviceContext->IACreate


		UINT offset = 0;
	pDeviceContext->IASetVertexBuffer(0, 1, &pVertexBuffer, &stride, &offset);


	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
};

	hr = pDevice->CreateInputLayout(pLayout, ARRAYSIZE(layout),pVertex

	
		pDeviceContext->IASet





	return false;
}
