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
	applicationName = L"Engine20 - RasterizerState";
	wndStyle = WS_OVERLAPPEDWINDOW;

	pApp = this;

	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;

	
}

DXApp::~DXApp()
{
	// �޸� ����.
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pRenderTargetView);
	Memory::SafeRelease(pSwapChain);

	Memory::SafeRelease(cBuffer);

	Memory::SafeRelease(lightBuffer);

	Memory::SafeRelease(depthStencilView);
	Memory::SafeRelease(depthStencilBuffer);

	for (int ix = 0; ix < meshes.size(); ++ix)
	{
		meshes[ix].Release();
	}
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
			gameTimer.UpdateTimer(this);

			// ��Ÿ Ÿ��(������ �ð�) ���.
			double deltaTime = gameTimer.DeltaTime();

			ProcessInput(deltaTime);

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
	//if (InitTransformation() == false) return false;

	// ����Ʈ ���� �ʱ�ȭ.
	if (InitLightCB() == false)
		return false;

	// �Է� �ʱ�ȭ.
	if (!InitInput())
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
	wc.lpszMenuName = NULL;
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

bool DXApp::InitInput()
{
	input = InputClass(hinstance, hwnd);
	
	if (!input.InitInput()) 
		return false;

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
	// �޽� ���� ����.
	InitMeshInfo();

	for (int ix = 0; ix < meshes.size(); ++ix)
	{
		Mesh* mesh = &meshes[ix];

		if (!CompileShader(mesh)) return false;

		if (!InitVertexBuffer(mesh)) return false;

		if (!InitIndexBuffer(mesh)) return false;

		if (!InitInputLayout(mesh)) return false;

		if (!mesh->CreateRasterizerState(pDevice, D3D11_FILL_WIREFRAME, D3D11_CULL_BACK))
			return false;

		mesh->BindRasterizerState(pDeviceContext);

		// ������ �̾ �׸� ��� ����.
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (!mesh->LoadTextures(pDevice)) return false;

		if (!mesh->CreateSamplerState(pDevice)) return false;

		InitTransformation(mesh,&camera);

		InitLightCB();

		BindLightCB();
	}

	SetViewport();
	

	return true;
}

void DXApp::InitMeshInfo()
{
	// ��(FBX) �̸�.
	LPCSTR fbxNameTPP 
		= "Resources//Models//HeroTPP.fbx";
	LPCSTR fbxNameBox
		= "Resources//Models//SK_CharM_Cardboard.fbx";
	LPCSTR fbxNameSphere
		= "Resources//Models//sphere.fbx";

	// �ؽ�ó �̸�.
	Texture tppDiffuseMap;
	tppDiffuseMap.fileName 
		= L"Resources//Textures//T_Chr_FPS_D.png";
	
	Texture tppNormalMap;
	tppNormalMap.fileName
		= L"Resources//Textures//T_Chr_FPS_N.png";

	Texture boxDiffuseMap;
	boxDiffuseMap.fileName
		= L"Resources//Textures//Char_M_Cardboard_D.png";
	
	Texture boxNormalMap;
	boxNormalMap.fileName
		= L"Resources//Textures//Char_M_Cardboard_N.png";

	Texture cubeMap;
	cubeMap.fileName 
		= L"Resources//Textures//LancellottiChapel.dds";


	// ��(Mesh).
	Mesh tppDiffuse(
		fbxNameTPP, 
		L"Shaders//DiffuseVS.fx", 
		L"Shaders//DiffusePS.fx"
	);
	// �ؽ�ó �߰�.
	tppDiffuse.AddTexture(tppDiffuseMap);
	tppDiffuse.SetPosition(XMFLOAT3(0.0f, -90.0f, 0.0f));
	tppDiffuse.SetRotation(XMFLOAT3(-90.0f, 180.0f, 0.0f));

	// �޽� �迭�� �߰�.
	meshes.push_back(tppDiffuse);

	Mesh tppNormal(
		fbxNameTPP, 
		L"Shaders//NormalVS.fx", 
		L"Shaders//NormalPS.fx");
	tppNormal.AddTexture(tppDiffuseMap);
	tppNormal.AddTexture(tppNormalMap);
	tppNormal.SetPosition(XMFLOAT3(-150.0f, -90.0f, 0.0f));
	tppNormal.SetRotation(XMFLOAT3(-90.0f, 180.0f, 0.0f));
	meshes.push_back(tppNormal);

	Mesh boxNormal(
		fbxNameBox, 
		L"Shaders//NormalVS.fx", 
		L"Shaders//NormalPS.fx");
	boxNormal.AddTexture(boxDiffuseMap);
	boxNormal.AddTexture(boxNormalMap);
	boxNormal.SetPosition(XMFLOAT3(150.0f, -90.0f, 0.0f));
	boxNormal.SetRotation(XMFLOAT3(-90.0f, 180.0f, 0.0f));
	meshes.push_back(boxNormal);
}

bool DXApp::CompileShader(Mesh * mesh)
{
	if (!mesh->CompileShaders(pDevice)) return false;
	return true;
}

void DXApp::BindShader(Mesh * mesh)
{
	mesh->BindShaders(pDeviceContext);
}

bool DXApp::InitVertexBuffer(Mesh * mesh)
{
	// ���� ������ FBX�κ��� �ε�.
	HRESULT hr;
	hr = FBXLoader::LoadFBX(
		mesh->GetFBXName(),
		mesh->GetVertexArray(),
		mesh->GetIndexArray()
	);
	if (FAILED(hr)) return false;

	// ���� ���� ����.
	if (!mesh->CreateVertexBuffer(pDevice)) 
		return false;

	return true;
}

void DXApp::BindVertexBuffer(Mesh * mesh)
{
	mesh->BindVertexBuffer(pDeviceContext);
}

bool DXApp::InitIndexBuffer(Mesh * mesh)
{
	if (!mesh->CreateIndexBuffer(pDevice)) 
		return false;

	return true;
}

void DXApp::BindIndexBuffer(Mesh * mesh)
{
	mesh->BindIndexBuffer(pDeviceContext);
}

bool DXApp::InitInputLayout(Mesh * mesh)
{
	if (!mesh->CreateInputLayout(pDevice))
		return false;

	return true;
}

void DXApp::BindInputLayout(Mesh * mesh)
{
	mesh->BindInputLayout(pDeviceContext);
}


bool DXApp::InitTransformation(Mesh * mesh, CameraClass* camera)
{
	// ī�޶� ��ġ ����.
	camera->SetPosition(XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f));

	// ��� ����.
	mesh->SetWVPMatrices(
		mesh->GetWorldMatrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix()
	);

	InitWVPBuffer(mesh);

	pDeviceContext->VSSetConstantBuffers(0, 1, &cBuffer);

	return true;
}

bool DXApp::InitWVPBuffer(Mesh * mesh)
{
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
	cbData.pSysMem = &mesh->GetWVPMatrices();

	// ���� ����.
	HRESULT hr = pDevice->CreateBuffer(&cbDesc, &cbData, &cBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"��� ���� ���� ����.", L"����", MB_OK);
		return false;
	}

	// WVP�� ������� ���ε�.
	//pDeviceContext->VSSetConstantBuffers(0, 1, &cBuffer);

	return true;
}

void DXApp::UpdateWVPBuffer(
	Mesh * mesh, 
	CameraClass* camera)
{
	// WVP ��� �� ����.
	mesh->SetWVPMatrices(
		mesh->GetWorldMatrix(),
		camera->GetViewMatrix(), 
		camera->GetProjectionMatrix()
	);

	// ���ŵ� ��� ������ ���ε��� ���� ������Ʈ.
	pDeviceContext->UpdateSubresource(cBuffer, 0, 0, &mesh->GetWVPMatrices(), 0, 0);
}

bool DXApp::InitLightCB()
{
	// ����Ʈ ��� ���ۿ� ����ü ����.
	CBLight cbLight;
	ZeroMemory(&cbLight, sizeof(cbLight));
	cbLight.lightPosition = XMVectorSet(500.0f, 500.0f, -500.0f, 1.0f);
	cbLight.cameraPosition = camera.GetPosition();


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

void DXApp::UpdateLightCB(Mesh * mesh)
{
	pDeviceContext->UpdateSubresource(lightBuffer, 0, 0, &mesh->GetLightInfo(), 0, 0);
}

void DXApp::BindLightCB()
{
	pDeviceContext->VSSetConstantBuffers(1, 1, &lightBuffer);
}

void DXApp::SetViewport()
{
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

	camera.SetAspecRation(clientWidth, clientHeight);
}