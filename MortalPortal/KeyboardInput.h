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

	DirectX::XMFLOAT2 GetDirection(unsigned int playerNr);
	unsigned int GetButtonState(unsigned int playerNr);
private:
	char keys[5];
	int returnColorState = 1;
	bool keyPressed = false;
};

