#include "Engine.h"

Engine::Engine(HINSTANCE hinstance) 
	: DXApp(hinstance)
{

}

Engine::~Engine()
{

}

bool Engine::Init()
{
	if (DXApp::Init() == false) 
		return false;

	return true;
}

void Engine::Update(double deltaTime)
{
	RotateObject(deltaTime);				// ��ü ȸ��.
}

void Engine::Render()
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

void Engine::DrawMesh()
{
	for (int ix = 0; ix < meshes.size(); ++ix)
	{
		Mesh* mesh = &meshes[ix];

		BindShader(mesh);
		BindVertexBuffer(mesh);
		BindIndexBuffer(mesh);
		BindInputLayout(mesh);

		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mesh->BindTextures(pDeviceContext);
		mesh->BindSamplerState(pDeviceContext);

		UpdateWVPBuffer(mesh);

		UpdateLightCB(mesh);
		mesh->DrawMesh(pDeviceContext);
	}
}

float rotY = 0.01f;		// ȸ�� ���� �����.
float rotX = 0.01f;		// ȸ�� ���� �����.
float rotZ = 0.01f;		// ȸ�� ���� �����.
//float pi = 3.141592f;
float doublePI = XM_PI * 2.0f;
void Engine::RotateObject(double deltaTime)
{
	// ������ ���� 0.01���� ����.
	/*rotY += XMConvertToRadians(0.01f);
	rotX += XMConvertToRadians(0.01f);
	rotZ += XMConvertToRadians(0.01f);*/
	rotY += XMConvertToRadians(30.0f * deltaTime);
	rotX += XMConvertToRadians(30.0f * deltaTime);
	rotZ += XMConvertToRadians(30.0f * deltaTime);
	if (rotY >= doublePI)		// �ѹ��� ���� 0���� ����.
		rotY = 0.0f;
	if (rotX >= doublePI)
		rotX = 0.0f;
	if (rotZ >= doublePI)
		rotZ = 0.0f;

	// ȸ�� ���.
	XMMATRIX rotationX = XMMatrixRotationX(0.0f);
	XMMATRIX rotationY = XMMatrixRotationY(rotY);
	XMMATRIX rotationZ = XMMatrixRotationZ(0.0f);
	XMMATRIX rotation = rotationZ * rotationX * rotationY;

	// �̵� ���.
	XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	// ������(Scale ���).
	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	// ���� ��� ����.
	worldMatrix = XMMatrixIdentity();
	worldMatrix = scale * rotation * translation;

	// ���̴� ������ۿ� ������ ������ ����.
	CBPerObject cbData;
	ZeroMemory(&cbData, sizeof(cbData));
	cbData.world = XMMatrixTranspose(worldMatrix);
	cbData.view = XMMatrixTranspose(viewMatrix);
	cbData.projection = XMMatrixTranspose(projectionMatrix);

	// ������ ����.
	pDeviceContext->UpdateSubresource(cBuffer, 0, 0, &cbData, 0, 0);
}
