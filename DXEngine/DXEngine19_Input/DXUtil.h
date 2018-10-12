#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <xnamath.h>

#include <fbxsdk.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "libfbxsdk.lib")

// �޸� ���� ���� �Լ�.
namespace Memory
{
	template <class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = NULL;
		}
	}

	template <class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = NULL;
		}
	}

	template <class T> void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = NULL;
		}
	}
}

// ���� ����.
struct Vertex
{
	XMFLOAT3 position;		// ���� ��ġ ����.
	XMFLOAT2 texCoord;		// �ؽ�ó ��ǥ(UV).
	XMFLOAT3 normal;		// ��� ����.
	XMFLOAT3 tangent;		// ź��Ʈ.
	XMFLOAT3 binormal;		// ���̳��.

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

struct Texture
{
public:
	Texture() { ZeroMemory(this, sizeof(Texture)); }
	~Texture() { Memory::SafeRelease(textureResource); }

	LPCWSTR fileName;
	ID3D11ShaderResourceView* textureResource = NULL;
};

bool IsError(HRESULT hr, LPCWSTR errorMsg);

extern float Rad2Deg;
extern float Deg2Rad;