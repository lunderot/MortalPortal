#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdexcept>

#include "Input.h"

#pragma comment(lib,"Xinput9_1_0.lib")


class ControllerInput :
	public Input
{
private:
	unsigned int id;
	XINPUT_STATE state;
	
	int returnColorState = 1;
	bool keyPressed = false;
public:
	ControllerInput(unsigned int playerNr);
	~ControllerInput();

	DirectX::XMFLOAT2 GetDirection();
	bool GetButtonState();
	bool GetButtonEnterState();
	bool GetButtonUpState();
	bool GetButtonDownState();
	bool GetButtonStartState();

};

