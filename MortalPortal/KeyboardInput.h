#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdexcept>

#include "Input.h"

class KeyboardInput :
	public Input
{
public:
	KeyboardInput();
	~KeyboardInput();

	DirectX::XMFLOAT2 GetDirection();
};

