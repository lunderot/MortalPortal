#include "KeyboardInput.h"


KeyboardInput::KeyboardInput()
{
}


KeyboardInput::~KeyboardInput()
{
}

DirectX::XMFLOAT2 KeyboardInput::GetDirection()
{
	DirectX::XMFLOAT2 returnValue(0, 0);

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

	returnValue.x /= magnitude;
	returnValue.y /= magnitude;

	return returnValue;
}
