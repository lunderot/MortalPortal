#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdexcept>

#include "Input.h"

class KeyboardInput :
	public Input
{
public:
	 KeyboardInput(char keys[]);
	~KeyboardInput();

	DirectX::XMFLOAT2 GetDirection();
	bool GetButtonState();
	bool GetButtonEnterState();
	bool GetButtonUpState();
	bool GetButtonDownState();
	bool GetButtonStartState();
private:
	char keys[5];
	int returnColorState;
	bool keyPressed;
};

