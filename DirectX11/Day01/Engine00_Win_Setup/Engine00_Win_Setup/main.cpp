#include <windows.h>

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdLine)
{
	// 
	WNDCLASSEX wc;
	ZeroMemory(&wc,sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(wc);
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = NULL;
	wc.lpszClassName = L"WindowsClass";
	wc.lpfnWndProc = WinProc;

	if (!RegisterClassEx(&wc))
		return -1;

	HWND hWnd;
	hWnd = CreateWindow(L"WindowsClass",L"Engine00 - Win Setup",WS_OVERLAPPEDWINDOW,0,0,800,600,NULL,NULL,hInstance,NULL);

	if (hWnd == NULL)
		return -1;

	ShowWindow(hWnd, SW_SHOW);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}