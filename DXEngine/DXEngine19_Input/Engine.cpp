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


void Engine::ProcessInput(double deltaTime)
{
	if (input.DetectKeyboard(DIK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	if (input.DetectKeyboard(DIK_LEFT) || input.DetectKeyboard(DIK_A))
	{
		// ī�޶� ���� �̵�.
		camera.MoveRight(-deltaTime);
	}

	if (input.DetectKeyboard(DIK_RIGHT) || input.DetectKeyboard(DIK_D))
	{
		camera.MoveRight(deltaTime);
	}

	if (input.DetectKeyboard(DIK_UP) || input.DetectKeyboard(DIK_W))
	{
		camera.MoveForward(deltaTime);
	}

	if (input.DetectKeyboard(DIK_DOWN) || input.DetectKeyboard(DIK_S))
	{
		camera.MoveForward(-deltaTime);
	}

	if (input.DetectKeyboard(DIK_Q))
	{
		camera.MoveUp(deltaTime);
	}

	if (input.DetectKeyboard(DIK_E))
	{
		camera.MoveUp(-deltaTime);
	}

	if (input.DetectMouse(currentMouseState))
	{
		// ī�޶� ȸ��.
		camera.Yaw(currentMouseState.lX * deltaTime);
		camera.Pitch(currentMouseState.lY * deltaTime);
	}
}

void Engine::Update(double deltaTime)
{
	camera.UpdateCamera();
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

		UpdateWVPBuffer(mesh, &camera);

		UpdateLightCB(mesh);
		mesh->DrawMesh(pDeviceContext);
	}
}
