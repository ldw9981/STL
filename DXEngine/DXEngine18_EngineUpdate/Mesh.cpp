#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(LPCWSTR fbxName, LPCWSTR vsName, LPCWSTR psName)
{
	this->fbxName = fbxName;
	vertexShader = VertexShader(vsName);
	pixelShader = PixelShader(psName);
}


Mesh::~Mesh()
{
}

bool Mesh::CompileShaders(ID3D11Device* device)
{
	if (!vertexShader.CompileShader())
		return false;

	if (!pixelShader.CompileShader())
		return false;

	if (!vertexShader.CreateShader(device))
		return false;

	if (!pixelShader.CreateShader(device))
		return false;

	return false;
}

void Mesh::BindShaders(ID3D11DeviceContext * deviceContext)
{
	vertexShader.BindShader(deviceContext);
	pixelShader.BindShader(deviceContext);
}

bool Mesh::CreateVertexBuffer(ID3D11Device * device)
{
	// 정점 개수 설정.
	int nVertices = vertices.size();

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	// sizeof(vertices) / sizeof(Vertex).
	vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 배열 데이터 할당.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(vbData));
	vbData.pSysMem = &vertices[0];

	// 정점 버퍼 생성.
	HRESULT hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"정점 버퍼 생성 실패.", L"오류.", MB_OK);
		return false;
	}
	return true;
}

void Mesh::BindVertexBuffer(ID3D11DeviceContext * deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 정점 버퍼 바인딩.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

bool Mesh::CreateIndexBuffer(ID3D11Device * device)
{
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = sizeof(DWORD) * indices.size();
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = &indices[0];

	// 인덱스 버퍼 생성.
	HRESULT hr = device->CreateBuffer(&ibDesc, &ibData, &indexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"인덱스 버퍼 생성 실패.", L"오류.", MB_OK);
		return false;
	}
	return true;
}

void Mesh::BindIndexBuffer(ID3D11DeviceContext * deviceContext)
{	// 인덱스 버퍼 바인딩(binding).
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

bool Mesh::CreateInputLayout(ID3D11Device * device)
{
	// 입력 레이아웃 생성.
	HRESULT hr = device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc),
		vertexShader.GetShaderBuffer()->GetBufferPointer(), vertexShader.GetShaderBuffer()->GetBufferSize() , &inputLayout);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"입력 레이아웃 생성 실패.", L"오류.", MB_OK);
		return false;
	}

	
}

void Mesh::BindInputLayout(ID3D11DeviceContext * deviceContext)
{
	// 입력 레이아웃 바인딩.
	deviceContext->IASetInputLayout(inputLayout);
}

void Mesh::AddTexture(Texture newTexture)
{
	pixelShader.AddTexture(newTexture);
}

void Mesh::DrawMesh(ID3D11DeviceContext * deviceContext)
{
	deviceContext->DrawIndexed(indices.size(), 0, 0);
	
}

void Mesh::SetPosition(XMFLOAT3 data)
{
	position = data;
}

XMFLOAT3 Mesh::GetPosition() const
{
	return position;
}

void Mesh::SetRotation(XMFLOAT3 data)
{
	rotation = data;
}

XMFLOAT3 Mesh::GetRotation() const
{
	return rotation;
}

void Mesh::SetScale(XMFLOAT3 data)
{
	scale = data;
}

XMFLOAT3 Mesh::GetScale() const
{
	return scale;
}

XMMATRIX Mesh::GetScaleMatrix()
{
	return XMMatrixScaling(scale.x,scale.y,scale.z);
}

XMMATRIX Mesh::GetRotationMatrix()
{
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(rotation.x));
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	XMMATRIX rotZ = XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	return rotZ * rotY * rotX;
}

XMMATRIX Mesh::GetTranslationMatrix()
{
	return XMMatrixTranslation(position.x, position.y, position.z );
}
