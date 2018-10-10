#pragma once
#include "DXUtil.h"
#include <vector>
#include "PixelShader.h"
#include "VertexShader.h"

class Mesh
{
public:
	Mesh();
	Mesh(LPCWSTR fbxName,LPCWSTR vsName,LPCWSTR psName);
	~Mesh();

	bool CompileShaders(ID3D11Device* device);
	void BindShaders(ID3D11DeviceContext* deviceContext);

	bool CreateVertexBuffer(ID3D11Device* device);
	void BindVertexBuffer(ID3D11DeviceContext* deviceContext);

	bool CreateIndexBuffer(ID3D11Device* device);
	void BindIndexBuffer(ID3D11DeviceContext* deviceContext);

	// ÀÔ·Â ·¹ÀÌ¾Æ¿ô
	bool CreateInputLayout(ID3D11Device* device);
	void BindInputLayout(ID3D11DeviceContext* deviceContext);

	void AddTexture(Texture newTexture);

	void DrawMesh(ID3D11DeviceContext* deviceContext);

	void SetPosition(XMFLOAT3 data);
	XMFLOAT3 GetPosition() const;
	void SetRotation(XMFLOAT3 data);
	XMFLOAT3 GetRotation() const;
	void SetScale(XMFLOAT3 data);
	XMFLOAT3 GetScale() const;

	XMMATRIX GetScaleMatrix();
	XMMATRIX GetRotationMatrix();
	XMMATRIX GetTranslationMatrix();


	LPCWSTR GetFBXName() const { return fbxName;  }
	std::vector<Vertex>* GetVertexArray() { return &vertices; }
	int GetVertexCount() const { return vertices.size(); }
	std::vector<DWORD>* GetIndexArray() { return &indices; }
	int GetIndexCount() const { return indices.size(); }

	D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc() { return inputLayoutDesc; }
private:
	LPCWSTR fbxName;		
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[5] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// ¹öÆÛ
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11InputLayout* inputLayout;

	// ¼ÎÀÌ´õ (Á¤Á¡/ÇÈ¼¿)
	VertexShader vertexShader;
	PixelShader pixelShader;

	// ¼ÎÀÌ´õ (Á¤Á¡/ÇÈ¼¿)
	// Æ®·£½ºÆûÁ¤º¸
	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
};

