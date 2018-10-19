#include <Windows.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <fbxsdk.h>
#include <dinput.h>
using namespace std;

#pragma comment(lib, "libfbxsdk.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


#define DIRECTINPUT_VERSION 0x0800

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

// ��� ���� ����ü - ����Ʈ ����.
struct CBLight
{
	XMVECTOR lightPosition;
	XMVECTOR cameraPosition;
};

CBPerObject cbMatrices;
CBLight cbLight;


HWND hwnd;
HINSTANCE hInstance;
UINT clientWidth = 800;
UINT clientHeight = 600;
LPCWSTR applicationName = PROJECT_NAME;
DWORD wndStyle = WS_OVERLAPPEDWINDOW;

// Device , Context
ID3D11Device* pDevice;
ID3D11DeviceContext* pDeviceContext;
IDXGISwapChain* pSwapChain;
ID3D11RenderTargetView* pRenderTargetView;

// ����/���ٽ� ����.
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;

// Geometry , Shader
ID3D11Buffer* vertexBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11InputLayout* vertexInputLayout;
int nVertices;
ID3D11Buffer* indexBuffer;
int nIndices;

std::vector<Vertex> vertices;		// ���� �迭.
std::vector<DWORD> indices;			// �ε��� �迭.







ID3D11Buffer* cBuffer;
ID3D11Buffer* lightBuffer;		// ������� - ����Ʈ��.

// Texture
ID3D11ShaderResourceView* pTexture;
ID3D11SamplerState* pSamplerState;
FbxManager* fbxManager;		// FBX �δ�.

//��ǲ
IDirectInputDevice8* diKeyboard;
IDirectInputDevice8* diMouse;

DIMOUSESTATE currentMouseState;
DIMOUSESTATE prevMouseState;
LPDIRECTINPUT8 directInput;

// Ÿ�̸� ���� ����.
double countsPerSecond = 0.0;
__int64 countStart = 0;

int frameCount = 0;
int fps = 0;

__int64 prevFrameTime = 0;

//ī�޶�����
float fovY;				//�þ߰�.
float aspectRatio;		// ȭ�� ��Ⱦ��.
float nearZ =1.0f ;				// ����� �Ÿ�.
float farZ=1000.f ;				// ����� �Ÿ�.

float yaw = 0.0f;		// yaw ȸ�� ��.
float pitch = 0.0f;		// pitch ȸ�� ��.

float moveSpeed = 200.0f;		// ī�޶� �̵� �ӵ�.
float rotationSpeed = 30.0f;		// ī�޶� ȸ�� �ӵ�.

bool modifiedProjection = false;

// Transform
XMMATRIX worldMatrix;

//ī�޶�
XMMATRIX viewMatrix;
XMMATRIX projectionMatrix;

XMVECTOR cameraPosition;
XMVECTOR cameraLook
= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR cameraRight
= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR cameraForward
= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

XMVECTOR defaultRight
= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR defaultUp
= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR defaultForward
= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);




// util
bool IsError(HRESULT hr, LPCWSTR errorMsg)
{
	if (hr < 0)
	{
		MessageBox(NULL, errorMsg, L"����", MB_OK);
		return true;
	}

	return false;
}

float Rad2Deg = 180.0f / XM_PI;
float Deg2Rad = XM_PI / 180.0f;



void StartTime()
{
	LARGE_INTEGER frequencyCount;

	// �ϵ���� ������(hz) ����.
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = (double)frequencyCount.QuadPart;

	// ƽ(Tick = Frame).
	QueryPerformanceCounter(&frequencyCount);
	countStart = frequencyCount.QuadPart;
}

double GetTime() 
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	double time
		= (double)((currentTime.QuadPart - countStart)
			/ countsPerSecond);

	return time;
}

double DeltaTime()
{
	// ƽ ���� ���� �ð� ���ϱ�.
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	// ���� �����ӿ��� ���� �����ӱ��� ƽ ������ �󸶳� �������� ���.
	__int64 tickCount = currentTime.QuadPart - prevFrameTime;

	// ���� ������ �ð� ����.
	prevFrameTime = currentTime.QuadPart;

	// ���� ó��.
	if (tickCount < 0.0f)
		tickCount = 0.0f;

	// ������ �ð��� ��(����) �ð����� ��ȯ.
	double deltaTime = (double)(tickCount / countsPerSecond);

	// ���� ó��.
	if (deltaTime < 0.0)
		deltaTime = 0.0;

	return deltaTime;
}
void Reset()
{
	fps = frameCount;
	frameCount = 0;
	StartTime();

	// FPS �ֿܼ� ���.
	OutputDebugString(L"FPS: ");
	wchar_t buffer[256];
	_itow_s(fps, buffer, 10);
	OutputDebugString(buffer);
	OutputDebugString(L"\n");
}
void UpdateTimer()
{
	// ƽ ������ ������ �� ������Ű��.
	++frameCount;

	// 1�ʰ� ������ Ÿ�̸� �ʱ�ȭ.
	if (GetTime() > 1.0f)
		Reset();
}



int GetFPS()
{
	return fps;
}


XMFLOAT2 ReadUV(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
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

XMFLOAT3 ReadNormal(FbxMesh * mesh, int controlPointIndex, int vertexCounter)
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
HRESULT LoadFBX(const char * fileName, std::vector<Vertex>* pOutVertices, std::vector<DWORD>* pOutIndices)
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


bool InitInput()
{
	HRESULT hr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (IsError(hr, L"�Է� �ʱ�ȭ ����")) 
		return false;

	hr = directInput->CreateDevice(GUID_SysKeyboard, &diKeyboard, NULL);
	if (IsError(hr, L"Ű���� �ʱ�ȭ ����")) return false;

	hr = directInput->CreateDevice(GUID_SysMouse, &diMouse, NULL);
	if (IsError(hr, L"���콺 �ʱ�ȭ ����")) return false;

	hr = diKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (IsError(hr, L"Ű���� ���� ���� ����")) return false;

	hr = diMouse->SetDataFormat(&c_dfDIMouse);
	if (IsError(hr, L"���콺 ���� ���� ����")) return false;

	hr = diKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (IsError(hr, L"Ű���� ���� ���� ���� ����")) return false;

	hr = diMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (IsError(hr, L"���콺 ���� ���� ���� ����")) return false;

	return true;
}

bool DetectKeyboard(BYTE key)
{
	// Ű���� �Է� ������ ���� �迭 ����.
	BYTE keyboardState[256];
	//unsigned char keyboardState[256];

	// �Է� ȹ��.
	diKeyboard->Acquire();

	// Ű���� ��ġ ���� ������.
	diKeyboard->GetDeviceState(sizeof(keyboardState), (void**)&keyboardState);

	// Ű���� �Է� Ȯ��.
	if (keyboardState[key] & 0x80) 
		return true;

	return false;
}

bool DetectMouse(DIMOUSESTATE & currentMouseState)
{
	// �Է� ȹ��.
	diMouse->Acquire();

	// ���콺 ���� ������.
	diMouse->GetDeviceState(sizeof(currentMouseState),
		&currentMouseState);

	// ���콺 �Է��� �߻��ߴ��� Ȯ���ϴ� ����.
	bool retValue = false;

	// ���콺 �Է�.
	if (currentMouseState.lX != prevMouseState.lX
		|| currentMouseState.lY != prevMouseState.lY)
	{
		retValue = true;
	}

	// �Է� �� ����.
	prevMouseState = currentMouseState;

	return retValue;
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);

		}
	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);

	}
	return 0;

	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
bool InitWindow()
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = NULL;
	wc.lpszClassName = L"WindowClass";
	wc.lpfnWndProc = WinProc;

	if (!RegisterClassEx(&wc))
		return false;

	hwnd = CreateWindow(L"WindowClass", PROJECT_NAME,
		wndStyle, 0, 0, clientWidth, clientHeight, NULL, NULL,
		hInstance, NULL);

	if (hwnd == NULL)
		return false;

	ShowWindow(hwnd, SW_SHOW);
	return true;
}


bool InitLightCB()
{
	// ����Ʈ ��� ���ۿ� ����ü ����.
	CBLight cbLight;
	ZeroMemory(&cbLight, sizeof(cbLight));
	cbLight.lightPosition = XMVectorSet(500.0f, 500.0f, -500.0f, 1.0f);
	cbLight.cameraPosition = cameraPosition;

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

bool InitDepthStencilBuffer()
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

bool InitDirect3D()
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
void SetAspecRation(UINT width, UINT height)
{
	/*aspectRatio = static_cast<float>(width) /
		static_cast<float>(height);*/
	aspectRatio = (float)width / (float)height;
	modifiedProjection = true;
}
void SetViewport()
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

	SetAspecRation(clientWidth, clientHeight);
}

bool InitScene()
{
	// ���̴� ������.
	HRESULT hr;

	// ���� ���̴� �������ؼ� ���� ���̴� ���ۿ� ����.
	hr = D3DX11CompileFromFile(L"SpecularVS.fx", NULL, NULL,
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
	hr = D3DX11CompileFromFile(L"SpecularPS.fx", NULL, NULL,
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
	SetViewport();

	return true;
}


void UpdateProjectionMatrix()
{
	projectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	modifiedProjection = false;
}

void UpdateViewMatrix()
{
	viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraLook, cameraUp);
}


void SetPosition(XMVECTOR pos)
{
	cameraPosition = pos;
	UpdateViewMatrix();
}
void SetFovY(float value)
{
	fovY = value * Deg2Rad;
	modifiedProjection = true;

}

bool InitTransformation()
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
	SetPosition(XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f));
	// �� ��ȯ ��� ����.
	UpdateViewMatrix();

	// ���� ��ȯ ��� ����. (90�� -> ����).
	SetFovY(75.0f);
	//float fovY = XM_PI / 4.0f;		// ���� �þ߰� ����.
	//float fovY = 75.0f * 3.14f / 180.0f;	
	//float fovY = 45.0f * 3.14f / 180.0f;
	UpdateProjectionMatrix();

	// ���ۿ� ����.
	// DX�� ��켱 / HLSL�� ���켱.

	cbMatrices.world = XMMatrixTranspose(worldMatrix);
	cbMatrices.view = XMMatrixTranspose(viewMatrix);
	cbMatrices.projection = XMMatrixTranspose(projectionMatrix);

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
	cbData.pSysMem = &cbMatrices;

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

bool InitTexture()
{
	// �ؽ�ó ���� �ε�.
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		pDevice, L"T_Chr_FPS_D.png", NULL, NULL, &pTexture, NULL);
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

// �ʱ�ȭ �޼ҵ�.
bool Init()
{
	// Win32 �ʱ�ȭ.
	if (InitWindow() == false)
		return false;

	// Direct3D �ʱ�ȭ.
	if (InitDirect3D() == false)
		return false;

	// Ÿ�̸� �ʱ�ȭ.
	StartTime();

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

	// �Է� �ʱ�ȭ.
	if (!InitInput())
		return false;

	return true;
}

void ClearBackBuffer()
{
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

}

void Present()
{
	pSwapChain->Present(0, 0);
}

void DrawMesh()
{
	// ���ŵ� ��� ������ ���ε��� ���� ������Ʈ.
	cbMatrices.world = XMMatrixTranspose(worldMatrix);
	cbMatrices.view = XMMatrixTranspose(viewMatrix);
	cbMatrices.projection = XMMatrixTranspose(projectionMatrix);
	pDeviceContext->UpdateSubresource(cBuffer, 0, 0, &cbMatrices, 0, 0);

	pDeviceContext->DrawIndexed(nIndices, 0, 0);

	
}




void GetPlaneDistance(float & zN, float & zF)
{
	zN = nearZ;
	zF = farZ;
}

void SetPlaneDistance(float zN, float zF)
{
	nearZ = zN;
	farZ = zF;
}



void MoveForward(float direction)
{
	cameraPosition += direction * moveSpeed * cameraForward;
}

void MoveRight(float direction)
{
	cameraPosition += direction * moveSpeed * cameraRight;
}

void MoveUp(float direction)
{
	cameraPosition += direction * moveSpeed* cameraUp;
}

void Yaw(float angle)
{
	yaw += angle * rotationSpeed * 0.001f;
}

void Pitch(float angle)
{
	pitch += angle * rotationSpeed * 0.001f;
}





void UpdateCamera()
{
	if (modifiedProjection)
		UpdateProjectionMatrix();

	XMMATRIX rotMatrix
		= XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

	cameraLook
		= XMVector3TransformCoord(defaultForward, rotMatrix);
	cameraLook = XMVector3Normalize(cameraLook);

	XMMATRIX rotYTemp
		= XMMatrixRotationY(yaw);

	cameraRight
		= XMVector3TransformCoord(defaultRight, rotYTemp);
	cameraForward
		= XMVector3TransformCoord(defaultForward, rotYTemp);
	cameraUp
		= XMVector3TransformCoord(defaultUp, rotYTemp);

	cameraLook = cameraPosition + cameraLook;

	UpdateViewMatrix();
}




void ProcessInput(double deltaTime)
{
	if (DetectKeyboard(DIK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	if (DetectKeyboard(DIK_LEFT) || DetectKeyboard(DIK_A))
	{
		// ī�޶� ���� �̵�.
		MoveRight(-deltaTime);
	}

	if (DetectKeyboard(DIK_RIGHT) || DetectKeyboard(DIK_D))
	{
		MoveRight(deltaTime);
	}

	if (DetectKeyboard(DIK_UP) || DetectKeyboard(DIK_W))
	{
		MoveForward(deltaTime);
	}

	if (DetectKeyboard(DIK_DOWN) || DetectKeyboard(DIK_S))
	{
		MoveForward(-deltaTime);
	}

	if (DetectKeyboard(DIK_Q))
	{
		MoveUp(deltaTime);
	}

	if (DetectKeyboard(DIK_E))
	{
		MoveUp(-deltaTime);
	}

	if (DetectMouse(currentMouseState))
	{
		// ī�޶� ȸ��.
		Yaw(currentMouseState.lX * deltaTime);
		Pitch(currentMouseState.lY * deltaTime);
	}
}

void Update(double deltaTime)
{
	UpdateCamera();
}

void Render()
{
	// ���� ����.
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	//float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// ȭ�� ĥ�ϱ�.
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// ����/���ٽ� �� �����.
	pDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ���� �׸���. (Draw Call. ��ο� ��).
	//pDeviceContext->Draw(nVertices, 0);
	//pDeviceContext->DrawIndexed(nIndices, 0, 0);
	DrawMesh();

	// ����ü�� ��ü.
	pSwapChain->Present(1, 0);
}


LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(
	HINSTANCE hinstance,
	HINSTANCE prevInstance,
	LPSTR lpCmdLine,
	int nCmdLine)
{
	hInstance = hinstance;
	if (!Init())
		return false;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Ÿ�̸� ������Ʈ.
			UpdateTimer();
			// ��Ÿ Ÿ��(������ �ð�) ���.
			double deltaTime = DeltaTime();
			ProcessInput(deltaTime);
			// ���� ������Ʈ.
			Update(deltaTime);
			Render();		
		}
	}

	return 0;
}



