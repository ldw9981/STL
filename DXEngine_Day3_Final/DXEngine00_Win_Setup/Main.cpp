#include <Windows.h>

// ������ ���ν��� �Լ�.
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

// ���� �Լ�.
int WINAPI WinMain(
	HINSTANCE hinstance,
	HINSTANCE prevInstance,
	LPSTR lpCmdLine,
	int nCmdLine)
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
	wc.lpszClassName = NULL;
	wc.lpszClassName = L"WindowClass";
	wc.lpfnWndProc = WinProc;

	// Ŭ���� ���.
	if (!RegisterClassEx(&wc))
		return 0;

	// ������ ����.
	HWND hwnd;
	hwnd = CreateWindow(L"WindowClass", L"Engine00 - Win Setup",
		WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, 
		hinstance, NULL);

	// ���� ó��.
	if (hwnd == NULL) 
		return 0;

	// â ����.
	ShowWindow(hwnd, SW_SHOW);

	// �޽��� ó�� ����.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// ���� ����.
	while (msg.message != WM_QUIT)
	{
		// �޽��� ����.
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, 
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
				return 0;
			}
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}