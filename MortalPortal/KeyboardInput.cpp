#include "KeyboardInput.h"

#include <iostream>

KeyboardInput::KeyboardInput()
{
}


KeyboardInput::~KeyboardInput()
{
}

DirectX::XMFLOAT2 KeyboardInput::GetDirection()
{
	DirectX::XMFLOAT2 returnValue(0.0f, 0.0f);
	
	if (GetAsyncKeyState('W'))
	{
		returnValue.y += 1.0f;
	}
	if (GetAsyncKeyState('S'))
	{
		returnValue.y -= 1.0f;
	}
	if (GetAsyncKeyState('D'))
	{
		returnValue.x += 1.0f;
	}
	if (GetAsyncKeyState('A'))
	{
		returnValue.x -= 1.0f;
	}
	
	float magnitude = sqrt(returnValue.x*returnValue.x + returnValue.y*returnValue.y);

	if (magnitude > 0.0f)
	{
		returnValue.x /= magnitude;
		returnValue.y /= magnitude;
	}
	
	return returnValue;
}

unsigned int KeyboardInput::GetButtonState()
{
	short test = GetAsyncKeyState(VK_SPACE);

	if (keyPressed == false && test < 0)
	{
		returnColorState *= -1;
		keyPressed = true;
	}
	//else if (keyPressed == false && (test & 1) != 0)
	//{
	//	returnColorState = 0;
	//	keyPressed = true;
	//}
	else if(test >= 0)
	{
		keyPressed = false;
	}
	std::cout << test << std::endl;
	return returnColorState;
	//return GetAsyncKeyState(VK_SPACE);
}
