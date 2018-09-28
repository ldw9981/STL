#include <windows.h>
#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdLine)
{
	Engine engine(hInstance);

	if (engine.Init() == false)
		return -1;

	return engine.Run();
}
