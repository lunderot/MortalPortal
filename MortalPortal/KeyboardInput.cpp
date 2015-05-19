#include "KeyboardInput.h"

#include <iostream>

KeyboardInput::KeyboardInput(char keys[])
{
	this->keys[0] = keys[0];
	this->keys[1] = keys[1];
	this->keys[2] = keys[2];
	this->keys[3] = keys[3];
	this->keys[4] = keys[4];
	returnColorState = 1;
	keyPressed = false;
}


KeyboardInput::~KeyboardInput()
{
}

DirectX::XMFLOAT2 KeyboardInput::GetDirection()
{
	DirectX::XMFLOAT2 returnValue(0.0f, 0.0f);
	
	if (GetAsyncKeyState(keys[0]))
	{
		returnValue.y += 1.0f;
	}
	if (GetAsyncKeyState(keys[1]))
	{
		returnValue.y -= 1.0f;
	}
	if (GetAsyncKeyState(keys[2]))
	{
		returnValue.x -= 1.0f;
	}
	if (GetAsyncKeyState(keys[3]))
	{
		returnValue.x += 1.0f;
	}
	
	
	float magnitude = sqrt(returnValue.x*returnValue.x + returnValue.y*returnValue.y);

	if (magnitude > 0.0f)
	{
		returnValue.x /= magnitude;
		returnValue.y /= magnitude;
	}
	
	return returnValue;
}

bool KeyboardInput::GetButtonState()
{
	return GetAsyncKeyState(keys[4]);
}

bool KeyboardInput::GetButtonEnterState()
{
	return GetAsyncKeyState(VK_RETURN);
}

bool KeyboardInput::GetButtonUpState()
{
	return GetAsyncKeyState(VK_UP);
}

bool KeyboardInput::GetButtonDownState()
{
	return GetAsyncKeyState(VK_DOWN);
}

bool KeyboardInput::GetButtonStartState()
{
	return GetAsyncKeyState(VK_ESCAPE);
}
