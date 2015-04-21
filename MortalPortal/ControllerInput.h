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
public:
	ControllerInput();
	~ControllerInput();

	DirectX::XMFLOAT2 GetDirection();
	unsigned int GetButtonState();
};

