#include "DXApp.h"

// �۷ι� ����.
DXApp* pApp = NULL;

// �޽��� ó�� �ݹ� �Լ� (����, wrapper).
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	if (pApp != NULL) pApp->MSGProc(hwnd, msg, wParam, lParam);
	else return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hinstance)
{
	// �ʱ�ȭ.
	hwnd = NULL;
	this->hinstance = hinstance;
	clientWidth = 800;
	clientHeight = 600;
	applicationName = L"Engine04 - Index Buffer";
	wndStyle = WS_OVERLAPPEDWINDOW;

	pApp = this;

	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;

	vertexBuffer = NULL;
	vertexShader = NULL;
	pixelShader = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;
	vertexInputLayout = NULL;
}

DXApp::~DXApp()
{
	// �޸� ����.
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pRenderTargetView);
	Memory::SafeRelease(pSwapChain);

	Memory::SafeRelease(vertexBuffer);
	Memory::SafeRelease(vertexShader);
	Memory::SafeRelease(pixelShader);
	Memory::SafeRelease(vertexShaderBuffer);
	Memory::SafeRelease(pixelShaderBuffer);
	Memory::SafeRelease(vertexInputLayout);
}

// �޽��� ó�� ����.
int DXApp::Run()
{
	// �޽��� ó�� ����.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// ���� ����.
	while (msg.message != WM_QUIT)
	{
		// �޽��� ����.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// ���� ������Ʈ.
			Update();

			// ȭ�� �׸���.
			Render();
		}
	}

	return 0;
}

// �ʱ�ȭ �޼ҵ�.
bool DXApp::Init()
{
	// Win32 �ʱ�ȭ.
	if (InitWindow() == false)
		return false;

	// Direct3D �ʱ�ȭ.
	if (InitDirect3D() == false)
		return false;

	// ��� �ʱ�ȭ.
	if (InitScene() == false) 
		return false;

	return true;
}

LRESULT DXApp::MSGProc(HWND hwnd, UINT msg, 
	WPARAM wParam, LPARAM lParam)
{
	// �޽��� ó��.
	switch (msg)
	{
		//case WM_KEYUP
	case WM_KEYDOWN:
	{
		// ESC�� ������.
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

// â �ʱ�ȭ.
bool DXApp::InitWindow()
{
	// ������ Ŭ���� ����.
	WNDCLASSEX wc;
	// �ʱ�ȭ.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = NULL;
	wc.lpszClassName = L"WindowClass";
	wc.lpfnWndProc = WinProc;

	// Ŭ���� ���.
	if (!RegisterClassEx(&wc))
		return false;

	// ������ ����.
	//HWND hwnd;
	hwnd = CreateWindow(L"WindowClass", applicationName,
		wndStyle, 0, 0, clientWidth, clientHeight, NULL, NULL,
		hinstance, NULL);

	// ���� ó��.
	if (hwnd == NULL)
		return false;

	// â ����.
	ShowWindow(hwnd, SW_SHOW);

	return true;
}

bool DXApp::InitDirect3D()
{
	// �����.
	HRESULT hr;

	// ����ü�� �Ӽ� ���� ����ü ����.
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;	// ����ü�� ����� â �ڵ� ��.
	swapDesc.Windowed = true;		// â ��� ���� ����.
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �����(�ؽ�ó)�� ����/���� ũ�� ����.
	swapDesc.BufferDesc.Width = clientWidth;
	swapDesc.BufferDesc.Height = clientHeight;
	// ȭ�� �ֻ��� ����.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	// ���ø� ���� ����.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	// ��ġ �� ����ü�� ����.
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &pSwapChain, &pDevice,
		NULL, &pDeviceContext);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"��ġ ���� ����.", L"����.", MB_OK);
		return false;
	}

	// �����(�ؽ�ó).
	ID3D11Texture2D* pBackBufferTexture;
	hr = pSwapChain->GetBuffer(NULL,
		__uuidof(ID3D11Texture2D),
		(void**)&pBackBufferTexture);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"����� ���� ����.", L"����.", MB_OK);
		return false;
	}

	// ���� Ÿ�� ����.
	hr = pDevice->CreateRenderTargetView(
		pBackBufferTexture, NULL, &pRenderTargetView);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"���� Ÿ�� ���� ����.", L"����.", MB_OK);
		return false;
	}

	// ���� Ÿ�� ����.
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// ����� �ؽ�ó ����.
	if (pBackBufferTexture)
	{
		pBackBufferTexture->Release();
		pBackBufferTexture = NULL;
	}

	return true;
}

bool DXApp::InitScene()
{
	// ���̴� ������.
	HRESULT hr;

	// ���� ���̴� �������ؼ� ���� ���̴� ���ۿ� ����.
	hr = D3DX11CompileFromFile(L"EffectVS.fx", NULL, NULL,
		"main", "vs_4_0", NULL, NULL, NULL,
		&vertexShaderBuffer, NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(hwnd, L"���� ���̴� ������ ����.", L"����.", MB_OK);
		return false;
	}

	// ���� ���̴� ����.
	hr = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);

	if (FAILED(hr))
	{
		MessageBox(hwnd, L"���� ���̴� ���� ����.", L"����.", MB_OK);
		return false;
	}

	// ���� ���̴� �ܰ迡 ���ε�(����, ����)binding.
	pDeviceContext->VSSetShader(vertexShader, NULL, NULL);

	// �ȼ� ���̴� ������.
	hr = D3DX11CompileFromFile(L"EffectPS.fx", NULL, NULL,
		"main", "ps_4_0", NULL, NULL, NULL, &pixelShaderBuffer,
		NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(hwnd, L"�ȼ� ���̴� ������ ����.", L"����.", MB_OK);
		return false;
	}

	// �ȼ� ���̴� ����.
	hr = pDevice->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	if (FAILED(hr))
	{
		MessageBox(hwnd, L"�ȼ� ���̴� ���� ����.", L"����.", MB_OK);
		return false;
	}

	// �ȼ� ���̴� ����.
	pDeviceContext->PSSetShader(pixelShader, NULL, NULL);

	// ���� ������(�迭) ����.
	Vertex vertices[] = 
	{
		Vertex( XMFLOAT3(-0.5f, 0.5f, 0.5f), 
		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) ),

		Vertex( XMFLOAT3(0.5f, 0.5f, 0.5f), 
		XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) ),

		Vertex( XMFLOAT3(0.5f, -0.5f, 0.5f), 
		XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) ),

		Vertex( XMFLOAT3(-0.5f, -0.5f, 0.5f), 
		XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f))
	};

	// ���� ���� ����.
	nVertices = ARRAYSIZE(vertices);

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	// sizeof(vertices) / sizeof(Vertex).
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// �迭 ������ �Ҵ�.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(vbData));
	vbData.pSysMem = vertices;

	// ���� ���� ����.
	hr = pDevice->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"���� ���� ���� ����.", L"����.", MB_OK);
		return false;
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// ���� ���� ���ε�.
	pDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// �ε��� ���� ����.
	DWORD indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	// �ε��� ���� ����.
	nIndices = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(indices);
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = indices;

	// �ε��� ���� ����.
	hr = pDevice->CreateBuffer(&ibDesc, &ibData, &indexBuffer);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"�ε��� ���� ���� ����.", L"����.", MB_OK);
		return false;
	}

	// �ε��� ���� ���ε�(binding).
	pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �Է� ���̾ƿ�.
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �Է� ���̾ƿ� ����.
	hr = pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &vertexInputLayout);

	if (FAILED(hr))
	{
		MessageBox(hwnd, L"�Է� ���̾ƿ� ���� ����.", L"����.", MB_OK);
		return false;
	}

	// �Է� ���̾ƿ� ���ε�.
	pDeviceContext->IASetInputLayout(vertexInputLayout);

	// ������ �̾ �׸� ��� ����.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ����Ʈ ����.
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = clientWidth;
	viewport.Height = clientHeight;

	// ����Ʈ ����.
	pDeviceContext->RSSetViewports(1, &viewport);

	return true;
}