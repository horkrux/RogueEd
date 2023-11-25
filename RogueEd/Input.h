#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	bool IsLMBPressed();
	bool IsRMBPressed();
	bool IsWPressed();
	bool IsSPressed();
	bool IsAPressed();
	bool IsDPressed();
	long GetMouseDirectionX();
	long GetMouseDirectionY();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	int m_screenWidth;
	int m_screenHeight;

	int m_mouseX;
	int m_mouseY;

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
};

#endif
