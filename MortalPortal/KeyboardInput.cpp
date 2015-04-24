#include "KeyboardInput.h"

#include <iostream>

KeyboardInput::KeyboardInput(char keys[])
{
	this->keys[0] = keys[0];
	this->keys[1] = keys[1];
	this->keys[2] = keys[2];
	this->keys[3] = keys[3];
	this->keys[4] = keys[4];
}


KeyboardInput::~KeyboardInput()
{
}

DirectX::XMFLOAT2 KeyboardInput::GetDirection(unsigned int playerNr)
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

unsigned int KeyboardInput::GetButtonState(unsigned int playerNr)
{
	short test = GetAsyncKeyState(keys[4]);


	if (keyPressed == false && test < 0)
	{
		returnColorState *= -1;
		keyPressed = true;
		std::cout << test << std::endl;
	}
	else if (test >= 0)
	{
		keyPressed = false;
	}

	/*if (keyPressed == false && test < 0)
	{
		returnColorState *= -1;
		keyPressed = true;
	}
	else if (test >= 0)
	{
		keyPressed = false;
	}*/
	

	return returnColorState;
	//return GetAsyncKeyState(VK_SPACE);
}
