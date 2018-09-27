#include "Engine.h"




Engine::Engine(HINSTANCE hInstance)
	:DXApp(hInstance)
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

void Engine::Update()
{
}

void Engine::Render()
{
	// 색으로 클리어
	float color[4] = { 0.0f,0.5f,0.5f,1.0f };

	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	pSwapChain->Present(0, 0);
}
