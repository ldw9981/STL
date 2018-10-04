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
	applicationName = L"Engine14 - Normal Mapping";
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

	/*Memory::SafeDeleteArray(vertices);
	Memory::SafeDeleteArray(indices);*/

	Memory::SafeRelease(lightBuffer);

	Memory::SafeRelease(depthStencilView);
	Memory::SafeRelease(depthStencilBuffer);
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
			// Ÿ�̸� ������Ʈ.
			gameTimer.UpdateTimer();

			// ��Ÿ Ÿ��(������ �ð�) ���.
			double deltaTime = gameTimer.DeltaTime();

			// ���� ������Ʈ.
			Update(deltaTime);

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
	
	// Ÿ�̸� �ʱ�ȭ.
	gameTimer.StartTime();

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

	// ����/���ٽ� �� ����.
	if (InitDepthStencilBuffer() == false)
		return false;

	// ���� Ÿ�� ����.
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, depthStencilView);

	// ����� �ؽ�ó ����.
	if (pBackBufferTexture)
	{
		pBackBufferTexture->Release();
		pBackBufferTexture = NULL;
	}

	return true;
}

bool DXApp::InitDepthStencilBuffer()
{
	// ���� ������ ����.
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = clientWidth;
	depthDesc.Height = clientHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	// ����/���ٽ� ���� ����.
	HRESULT hr;
	hr = pDevice->CreateTexture2D(&depthDesc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����/���ٽ� ���� ���� ����.", L"����", MB_OK);
		return false;
	}

	// ����/���ٽ� �� ����.
	hr = pDevice->CreateDepthStencilView(
		depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����/���ٽ� �� ���� ����.", L"����", MB_OK);
		return false;
	}

	return true;
}

bool DXApp::InitScene()
{
	// ���̴� ������.
	HRESULT hr;

	// ���� ���̴� �������ؼ� ���� ���̴� ���ۿ� ����.
	hr = D3DX11CompileFromFile(L"NormalVS.fx", NULL, NULL,
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
	hr = D3DX11CompileFromFile(L"NormalPS.fx", NULL, NULL,
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

	// �� �ε�.
	/*if (LoadModel("cube.txt") == false)
		return false;*/
	hr = LoadFBX("HeroTPP.fbx", &vertices, &indices);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"FBX �ε� ����", L"����", MB_OK);
		return false;
	}

	// ����/�ε��� ���� ����.
	nVertices = vertices.size();
	nIndices = indices.size();

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
	vbData.pSysMem = &vertices[0];

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

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = sizeof(DWORD) * nIndices;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = &indices[0];

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
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

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
	/*vertices = new Vertex[nVertices];
	indices = new DWORD[nIndices];*/

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
		Vertex vertex(
			XMFLOAT3(x, y, z), 
			XMFLOAT2(u, v), 
			XMFLOAT3(nx, ny, nz));

		vertices.push_back(vertex);

		// �ε��� ���� ����.
		indices.push_back(ix);

		//// ���� ������ ����.
		//vertices[ix].position = XMFLOAT3(x, y, z);
		//vertices[ix].texCoord = XMFLOAT2(u, v);
		//vertices[ix].normal = XMFLOAT3(nx, ny, nz);

		//// �ε��� ���� ����.
		//indices[ix] = ix;
	}

	// ���� �ݱ�.
	fin.close();

	return true;
}

bool DXApp::InitTransformation()
{
	// ���� ��ȯ ��� ����.
	worldMatrix = XMMatrixIdentity();

	// ȸ��.
	XMMATRIX rotationX
		= XMMatrixRotationX(XMConvertToRadians(-90.0f));
	XMMATRIX rotationY
		= XMMatrixRotationY(XMConvertToRadians(180.0f));
	XMMATRIX rotation = rotationX * rotationY;
	// �̵�.
	XMMATRIX translation = XMMatrixTranslation(0.0f, -90.0f, 0.0f);
	worldMatrix = rotation * translation;

	// ī�޶� ���� ����.
	cameraPos = XMVectorSet(0.0f, 100.0f, -50.0f, 1.0f);
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
		pDevice, L"T_Chr_FPS_D.png", NULL, NULL, &pTexture, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�ؽ�ó �ε� ����", L"����", MB_OK);
		return false;	
	}

	// �ؽ�ó ���� �ε�.
	hr = D3DX11CreateShaderResourceViewFromFile(
		pDevice, L"T_Chr_FPS_N.png", NULL, NULL, &pNormalTexture, NULL);
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

	// �ؽ�ó ���ε�.
	pDeviceContext->PSSetShaderResources(1, 1, &pNormalTexture);

	// ���÷� ������Ʈ ���ε�.
	pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
	
	return true;
}

bool DXApp::InitLightCB()
{
	// ����Ʈ ��� ���ۿ� ����ü ����.
	CBLight cbLight;
	ZeroMemory(&cbLight, sizeof(cbLight));
	cbLight.lightPosition = XMVectorSet(500.0f, 500.0f, -500.0f, 1.0f);
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

HRESULT DXApp::LoadFBX(const char * fileName, std::vector<Vertex>* pOutVertices, std::vector<DWORD>* pOutIndices)
{
	// fbxManager�� �������� �ʾ����� ����.
	if (fbxManager == NULL)
	{
		// �ʱ� SDK Manager ����.
		fbxManager = FbxManager::Create();

		FbxIOSettings* pIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(pIOSettings);
	}

	FbxGeometryConverter gConverter(fbxManager);
	FbxImporter* importer
		= FbxImporter::Create(fbxManager, "");
	FbxScene* fbxScene
		= FbxScene::Create(fbxManager, "");

	// ������ �ʱ�ȭ.
	if (importer->Initialize(fileName, -1, fbxManager->GetIOSettings()) == false)
	{
		return E_FAIL;
	}

	// 3D �� ����Ʈ.
	if (importer->Import(fbxScene) == false)
	{
		return E_FAIL;
	}

	// ������ ����.
	importer->Destroy();

	// FBX Node �б�.
	FbxNode* fbxRootNode = fbxScene->GetRootNode();
	if (fbxRootNode == NULL)
		return E_FAIL;

	// �𵨸� ������ �⺻ ������ �ﰢ������ ����.
	gConverter.Triangulate(fbxScene, true);

	int childCount = fbxRootNode->GetChildCount();
	for (int ix = 0; ix < childCount; ++ix)
	{
		// �ڽ� ���� ��� �о����.
		FbxNode* fbxChildNode
			= fbxRootNode->GetChild(ix);

		// �Ӽ��� ���� ���� �н�.
		if (fbxChildNode->GetNodeAttribute() == NULL)
			continue;

		// ��� �Ӽ� Ÿ�� Ȯ��.
		FbxNodeAttribute::EType arrtType
			= fbxChildNode->GetNodeAttribute()->GetAttributeType();

		// ��� Ÿ���� �޽ð� �ƴ� ��� �н�.
		if (arrtType != FbxNodeAttribute::EType::eMesh)
			continue;

		// �޽� ���� �б�.
		FbxMesh* fbxMesh = fbxChildNode->GetMesh();

		// ��Ʈ�� ����Ʈ �迭 ������ (����).
		FbxVector4* vertices = fbxMesh->GetControlPoints();
		// ������ �� Ȯ��.
		int polyCount = fbxMesh->GetPolygonCount();
		int vertexCounter = 0;
		
		for (int jx = 0; jx < polyCount; ++jx)
		{
			// ������ ���� ���� ���� Ȯ��.
			int vertexCount = fbxMesh->GetPolygonSize(jx);

			for (int kx = 0; kx < vertexCount; ++kx)
			{
				// �ﰢ��(������) ��ȣ, ���� ��ȣ�� ���� ���� �б�.
				int vertexIndex = fbxMesh->GetPolygonVertex(jx, kx);

				// ���� ���� �б�.
				Vertex vertex;
				vertex.position.x = (float)vertices[vertexIndex].mData[0];
				vertex.position.y = (float)vertices[vertexIndex].mData[1];
				vertex.position.z = (float)vertices[vertexIndex].mData[2];

				// UV �б�.
				vertex.texCoord = ReadUV(fbxMesh, vertexIndex, vertexCounter);

				// ��� �б�.
				vertex.normal = ReadNormal(fbxMesh, vertexIndex, vertexCounter);

				// ź��Ʈ �б�.
				bool hasNoTangent
					= fbxMesh->GetElementTangentCount() < 1;
				if (hasNoTangent)
				{
					MessageBox(NULL, L"ź��Ʈ ����", L"����", MB_OK);
				}

				vertex.tangent = hasNoTangent ? XMFLOAT3(0.0f, 0.0, 0.0f) : ReadTangent(fbxMesh, vertexIndex, vertexCounter);

				// ���̳�� �б�.
				bool hasNoBinormal
					= fbxMesh->GetElementBinormalCount() < 1;
				if (hasNoBinormal)
					MessageBox(NULL, L"���̳�� ����", L"����", MB_OK);

				vertex.binormal = hasNoBinormal ? XMFLOAT3(0.0f, 0.0f, 0.0f) :
					ReadBinormal(fbxMesh, vertexIndex, vertexCounter);

				// ���� �迭�� ������ �߰�.
				pOutVertices->push_back(vertex);
				// �ε��� �迭�� ������ �߰�.
				pOutIndices->push_back(vertexCounter);
				++vertexCounter;
			}
		}
	}


	return S_OK;
}

XMFLOAT2 DXApp::ReadUV(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
{
	// UV�� �ִ��� Ȯ��.
	if (mesh->GetElementUVCount() < 1)
	{
		MessageBox(NULL, L"UV�� �����ϴ�.", L"����", MB_OK);
		return NULL;
	}

	// ��ȯ�� ������ ����.
	XMFLOAT2 texCoord(0.0f, 0.0f);

	// UV ��ü �迭 �б�.
	FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0);
	const bool isUsingIndex
		= vertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
	const int indexCount = isUsingIndex ? vertexUV->GetIndexArray().GetCount() : 0;

	// ��� Ȯ��.
	switch (vertexUV->GetMappingMode())
	{
		// ���� ������ ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexUV->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			/*if (isUsingIndex == true)
				index = vertexUV->GetIndexArray().GetAt(controlPointIndex);
			else
				index = controlPointIndex;*/

			// UV �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV �� ��ȯ.
			return texCoord;
		}

		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexUV->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			texCoord.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
			texCoord.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(index).mData[1];

			// UV �� ��ȯ.
			return texCoord;
		}

		default:
		{
			MessageBox(NULL, L"UV ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
	}

	return NULL;
}

XMFLOAT3 DXApp::ReadNormal(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
{
	XMFLOAT3 normal(0.0f, 0.0f, 0.0f);

	// ����� �ִ��� Ȯ��.
	if (mesh->GetElementNormalCount() < 1)
	{
		MessageBox(NULL, L"����� �����ϴ�.", L"����", MB_OK);
		return NULL;
	}

	FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	const bool isUsingIndex
		= vertexNormal->GetReferenceMode() != FbxGeometryElement::eDirect;
	const int indexCount = isUsingIndex ? vertexNormal->GetIndexArray().GetCount() : 0;

	switch (vertexNormal->GetMappingMode())
	{
		// ���� ������ ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� ��� ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexNormal->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			// ��� �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];

			// ��� �� ��ȯ.
			return normal;
		}

		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� ��� ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexNormal->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// ��� �� �о����. (DirectX�� FBX�� UV�� V��ǥ�� ���� �ݴ�.)
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];
			
			// ��� �� ��ȯ.
			return normal;
		}

		default:
		{
			MessageBox(NULL, L"��� ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
	}

	return NULL;
}

XMFLOAT3 DXApp::ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	// UV�� �ִ��� Ȯ��.
	if (mesh->GetElementTangentCount() < 1)
	{
		MessageBox(NULL, L"Tangent ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
		return NULL;
	}

	// ���Ͽ� ���� ����.
	XMFLOAT3 tangent(0.0f, 0.0f, 0.0f);
	// UV ��ü �迭 ������.
	FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
	const bool isUsingIndex = vertexTangent->GetReferenceMode() != FbxGeometryElement::eDirect;
	const int indexCount = isUsingIndex ? vertexTangent->GetIndexArray().GetCount() : 0;

	switch (vertexTangent->GetMappingMode())
	{
		// UV �Ӽ��� ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexTangent->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return tangent;
		}

		// UV �Ӽ��� ���� ������ ���.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������. ���׿�����.
			int index = isUsingIndex == true ? vertexTangent->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			tangent.x = (float)vertexTangent->GetDirectArray().GetAt(index).mData[0];
			tangent.y = (float)vertexTangent->GetDirectArray().GetAt(index).mData[1];
			tangent.z = (float)vertexTangent->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return tangent;
		}

		default:
		{
			MessageBox(NULL, L"Tangent ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
	}

	return NULL;
}

XMFLOAT3 DXApp::ReadBinormal(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
{
	// UV�� �ִ��� Ȯ��.
	if (mesh->GetElementBinormalCount() < 1)
	{
		MessageBox(NULL, L"Binormal ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
		return NULL;
	}

	// ���Ͽ� ���� ����.
	XMFLOAT3 binormal(0.0f, 0.0f, 0.0f);
	// UV ��ü �迭 ������.
	FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
	const bool isUsingIndex = vertexBinormal->GetReferenceMode() != FbxGeometryElement::eDirect;
	const int indexCount = isUsingIndex ? vertexBinormal->GetIndexArray().GetCount() : 0;

	switch (vertexBinormal->GetMappingMode())
	{
		// UV �Ӽ��� ������ ������ ���.
		case FbxGeometryElement::eByControlPoint:
		{
			// ���� UV ���� �о�� �ε��� ������.
			int index = isUsingIndex ? vertexBinormal->GetIndexArray().GetAt(controlPointIndex) : controlPointIndex;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return binormal;
		}

		// UV �Ӽ��� ���� ������ ���.
		case FbxGeometryElement::eByPolygonVertex:
		{
			// ���� UV ���� �о�� �ε��� ������. ���׿�����.
			int index = isUsingIndex == true ? vertexBinormal->GetIndexArray().GetAt(vertexCounter) : vertexCounter;

			// UV �� �о����. (DirectX�� FBX�� UV V��ǥ�� ���� �ݴ�).
			binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[0];
			binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[1];
			binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(index).mData[2];

			// UV �� ��ȯ.
			return binormal;
		}

		default:
		{
			MessageBox(NULL, L"Binormal ���� ��ȿ���� �ʽ��ϴ�", L"����", MB_OK);
			return NULL;
		}
	}

	return NULL;
}
