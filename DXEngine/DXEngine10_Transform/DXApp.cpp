#include "DXApp.h"
#include <fstream>

using namespace std;;

// �۷ι� ����.
DXApp* pApp = NULL;

// �޽��� ó�� �ݹ� �Լ� (����, wrapper).
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	if (pApp != NULL) return pApp->MSGProc(hwnd, msg, wParam, lParam);
	else return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hinstance)
{
	// �ʱ�ȭ.
	hwnd = NULL;
	this->hinstance = hinstance;
	clientWidth = 1600;
	clientHeight = 960;
	applicationName = L"Engine10 - Transform";
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

	Memory::SafeRelease(indexBuffer);
	Memory::SafeRelease(cBuffer);
	Memory::SafeRelease(pTexture);
	Memory::SafeRelease(pSamplerState);

	Memory::SafeDeleteArray(vertices);
	Memory::SafeDeleteArray(indices);

	Memory::SafeRelease(lightBuffer);
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

	// ���� ��ȯ ��� �ʱ�ȭ.
	if (InitTransformation() == false) 
		return false;

	// ����Ʈ ���� �ʱ�ȭ.
	if (InitLightCB() == false)
		return false;

	// �ؽ�ó �ʱ�ȭ.
	if (InitTexture() == false)
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
	hr = D3DX11CompileFromFile(L"DiffuseVS.fx", NULL, NULL,
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
	hr = D3DX11CompileFromFile(L"DiffusePS.fx", NULL, NULL,
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
	/*Vertex vertices[] = 
	{
		Vertex( XMFLOAT3(-0.5f, 0.5f, 0.5f), 
		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) ),

		Vertex( XMFLOAT3(0.5f, 0.5f, 0.5f), 
		XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),

		Vertex( XMFLOAT3(0.5f, -0.5f, 0.5f), 
		XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)),

		Vertex( XMFLOAT3(-0.5f, -0.5f, 0.5f), 
		XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),  XMFLOAT2(0.0f, 1.0f))
	};*/

	// ���� ���� ����.
	//nVertices = ARRAYSIZE(vertices);

	// �� �ε�.
	if (LoadModel("cube.txt") == false)
		return false;

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	// sizeof(vertices) / sizeof(Vertex).
	vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
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
	/*DWORD indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};*/

	// �ε��� ���� ����.
	//nIndices = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = sizeof(DWORD) * nIndices;
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

bool DXApp::LoadModel(const char * fileName)
{
	// ���� �б�.
	ifstream fin;
	fin.open(fileName);

	// ����� ���ȴ��� Ȯ��.
	if (fin.fail())
		return false;

	char input;					// ���Ͽ��� ���� ���� ����.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> nVertices;			// �ؽ�Ʈ ���Ͽ� ����� ���� ���� ����.
	nIndices = nVertices;		// �ε��� ������ ���� ������ �����ϰ� ����.

	// �迭 ���� Ȯ��.
	vertices = new Vertex[nVertices];
	indices = new DWORD[nIndices];

	// ���� ������ �̵�.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// �� �� �ǳ� �ٱ�.
	fin.get(input);
	fin.get(input);

	// �迭 ������ ����.
	for (int ix = 0; ix < nVertices; ++ix)
	{	
		float x, y, z, u, v, nx, ny, nz;			// ������ ���� ����.
		fin >> x >> y >> z;
		fin >> u >> v;
		fin >> nx >> ny >> nz;

		// ���� ������ ����.
		vertices[ix].position = XMFLOAT3(x, y, z);
		vertices[ix].texCoord = XMFLOAT2(u, v);
		vertices[ix].normal = XMFLOAT3(nx, ny, nz);

		// �ε��� ���� ����.
		indices[ix] = ix;
	}

	// ���� �ݱ�.
	fin.close();

	return true;
}

bool DXApp::InitTransformation()
{
	// ���� ��ȯ ��� ����.
	worldMatrix = XMMatrixIdentity();

	// ī�޶� ���� ����.
	cameraPos = XMVectorSet(-1.5f, 2.0f, -3.0f, 1.0f);
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	// �� ��ȯ ��� ����.
	viewMatrix = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);

	// ���� ��ȯ ��� ����. (90�� -> ����).
	//float fovY = XM_PI / 4.0f;		// ���� �þ߰� ����.
	float fovY = 75.0f * 3.14f / 180.0f;
	//float fovY = 45.0f * 3.14f / 180.0f;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovY, (float)clientWidth / (float)clientHeight, 1.0f, 1000.0f);

	// ���ۿ� ����.
	// DX�� ��켱 / HLSL�� ���켱.
	CBPerObject cData;
	ZeroMemory(&cData, sizeof(CBPerObject));
	// XMMatrixTranspose->��ġ��� ���ϴ� API.
	cData.world = XMMatrixTranspose(worldMatrix);
	cData.view = XMMatrixTranspose(viewMatrix);
	cData.projection = XMMatrixTranspose(projectionMatrix);

	// ���� ����.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(CBPerObject);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(cbData));
	cbData.pSysMem = &cData;

	// ���� ����.
	HRESULT hr = pDevice->CreateBuffer(&cbDesc, &cbData, &cBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"��� ���� ���� ����.", L"����", MB_OK);
		return false;
	}

	// ���ε�.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cBuffer);

	return true;
}

bool DXApp::InitTexture()
{
	// �ؽ�ó ���� �ε�.
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		pDevice, L"dx.jpg", NULL, NULL, &pTexture, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�ؽ�ó �ε� ����", L"����", MB_OK);
		return false;	
	}

	// ���÷� ������Ʈ.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// ���÷� ������Ʈ ����.
	hr = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���÷� ������Ʈ ���� ����", L"����", MB_OK);
		return false;
	}

	// �ؽ�ó ���ε�.
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	// ���÷� ������Ʈ ���ε�.
	pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
	
	return true;
}

bool DXApp::InitLightCB()
{
	// ����Ʈ ��� ���ۿ� ����ü ����.
	CBLight cbLight;
	ZeroMemory(&cbLight, sizeof(cbLight));
	cbLight.lightPosition = XMVectorSet(8.0f, 8.0f, -5.0f, 1.0f);
	cbLight.cameraPosition = cameraPos;

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(CBLight);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(cbData));
	cbData.pSysMem = &cbLight;

	// ��� ���� ����.
	HRESULT hr = pDevice->CreateBuffer(&cbDesc, &cbData, &lightBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����Ʈ ���� ���� ����", L"����", MB_OK);
		return false;
	}

	// ���ε�.
	pDeviceContext->VSSetConstantBuffers(1, 1, &lightBuffer);

	return true;
}