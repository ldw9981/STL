#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(HINSTANCE hinstance, HWND hwnd)
{
	this->hinstance = hinstance;
	this->hwnd = hwnd;
}

InputClass::~InputClass()
{
}

bool InputClass::InitInput()
{
	HRESULT hr;
	hr = DirectInput8Create(	hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (IsError(hr, L"�Է� �ʱ�ȭ ����")) 
		return false;

	hr = directInput->CreateDevice(GUID_SysKeyboard, &diKeyboard, NULL);
	if (IsError(hr, L"Ű���� �ʱ�ȭ ����")) return false;

	hr = directInput->CreateDevice(GUID_SysMouse, &diMouse, NULL);
	if (IsError(hr, L"���콺 �ʱ�ȭ ����")) return false;

	hr = diKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (IsError(hr, L"Ű���� ���� ���� ����")) return false;

	hr = diMouse->SetDataFormat(&c_dfDIMouse);
	if (IsError(hr, L"���콺 ���� ���� ����")) return false;

	hr = diKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (IsError(hr, L"Ű���� ���� ���� ���� ����")) return false;

	hr = diMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY);
	if (IsError(hr, L"���콺 ���� ���� ���� ����")) return false;

	return true;
}

bool InputClass::DetectKeyboard(BYTE key)
{
	// Ű���� �Է� ������ ���� �迭 ����.
	BYTE keyboardState[256];
	//unsigned char keyboardState[256];

	// �Է� ȹ��.
	diKeyboard->Acquire();

	// Ű���� ��ġ ���� ������.
	diKeyboard->GetDeviceState(sizeof(keyboardState), (void**)&keyboardState);

	// Ű���� �Է� Ȯ��.
	if (keyboardState[key] & 0x80) 
		return true;

	return false;
}

bool InputClass::DetectMouse(DIMOUSESTATE & currentMouseState)
{
	// �Է� ȹ��.
	diMouse->Acquire();

	// ���콺 ���� ������.
	diMouse->GetDeviceState(sizeof(currentMouseState),
		&currentMouseState);

	// ���콺 �Է��� �߻��ߴ��� Ȯ���ϴ� ����.
	bool retValue = false;

	// ���콺 �Է�.
	if (currentMouseState.lX != prevMouseState.lX
		|| currentMouseState.lY != prevMouseState.lY)
	{
		retValue = true;
	}

	// �Է� �� ����.
	prevMouseState = currentMouseState;

	return retValue;
}
