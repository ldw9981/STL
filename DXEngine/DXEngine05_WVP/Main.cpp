#include "Engine.h"

// ���� �Լ�.
int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE prevInstance,
	LPSTR lpCmdLine, int nCmdLine)
{
	// ���� ��ü ����.
	Engine engine(hinstance);

	// ���� �ʱ�ȭ.
	if (engine.Init() == false) 
		return 0;

	return engine.Run();
}