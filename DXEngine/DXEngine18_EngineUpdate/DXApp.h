#pragma once

#include <Windows.h>
#include "DXUtil.h"
#include "GameTimer.h"
#include <vector>
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "FBXLoader.h"

class DXApp
{
public:
	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	int Run();

	virtual bool Init();
	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;
	virtual LRESULT MSGProc(HWND, UINT, WPARAM, LPARAM);

protected:

	// Win32 설정 변수.
	HWND hwnd;
	HINSTANCE hinstance;
	UINT clientWidth;
	UINT clientHeight;
	LPCWSTR applicationName;
	DWORD wndStyle;

	// DirectX 변수.
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;

	ID3D11Buffer* vertexBuffer;		// 정점 버퍼.
	ID3D11VertexShader* vertexShader;		// 정점 셰이더.
	ID3D11PixelShader* pixelShader;			// 픽셀 셰이더.
	ID3DBlob* vertexShaderBuffer;		// 정점 셰이더 버퍼.
	ID3DBlob* pixelShaderBuffer;		// 픽셀 셰이더 버퍼.
	ID3D11InputLayout* vertexInputLayout;	// 입력 레이아웃.

	int nVertices;							// 정점 개수.
	//Vertex* vertices;						// 정점 배열.
	std::vector<Vertex> vertices;		// 정점 배열.


	ID3D11Buffer* indexBuffer;			// 인덱스(색인) 버퍼.
	int nIndices;								// 인덱스 개수.
	//DWORD* indices;						// 인덱스 배열.
	std::vector<DWORD> indices;			// 인덱스 배열.

	// 공간변환에 필요한 변수 선언.
	XMMATRIX worldMatrix;				// 월드 변환 행렬.
	XMMATRIX viewMatrix;				// 뷰 변환 행렬.
	XMMATRIX projectionMatrix;		// 투영 변환 행렬.

	XMVECTOR cameraPos;				// 카메라 위치 저장.
	XMVECTOR cameraTarget;			// 카메라가 바라볼 방향 저장.
	XMVECTOR cameraUp;				// 카메라 위 방향 저장.

	ID3D11Buffer* cBuffer;				// 상수버퍼.

	// 텍스처 / 샘플러 스테이트.
	ID3D11ShaderResourceView* pTexture;
	ID3D11SamplerState* pSamplerState;

	ID3D11ShaderResourceView* pNormalTexture;
	//ID3D11SamplerState* pNormalSamplerState;

	ID3D11Buffer* lightBuffer;		// 상수버퍼 - 라이트용.

	GameTimer gameTimer;			// 게임 타이머.

	FbxManager* fbxManager;		// FBX 로더.

	// 뎁스/스텐실 버퍼.
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

protected:

	// 초기화 함수(메소드).
	bool InitWindow();

	// DirectX 초기화 함수.
	bool InitDirect3D();

	// 뎁스 버퍼 초기화.
	bool InitDepthStencilBuffer();

	// DirectX 장면 초기화.
	bool InitScene();

	// 모델 정보 초기화 (큐브).
	bool LoadModel(const char* fileName);

	// 공간 변환 행렬 초기화.
	bool InitTransformation();

	// 텍스처 초기화.
	bool InitTexture();

	// 라이트 정보 초기화.
	bool InitLightCB();
};