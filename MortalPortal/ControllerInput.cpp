#include "ControllerInput.h"

using namespace DirectX;

ControllerInput::ControllerInput()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&state, sizeof(state));

		DWORD result = XInputGetState(i, &state);
		if (result == ERROR_SUCCESS)
		{
			id = i;
			break;
		}
		else
		{
			throw std::runtime_error("No controller found");
		}
	}
}


ControllerInput::~ControllerInput()
{
}

XMFLOAT2 ControllerInput::GetDirection()
{
	XMFLOAT2 returnValue;

	XInputGetState(id, &state);

	float leftX = state.Gamepad.sThumbLX;
	float leftY = state.Gamepad.sThumbLY;

	float magnitude = sqrt(leftX*leftX + leftY*leftY);

	returnValue.x = leftX / magnitude;
	returnValue.y = leftY / magnitude;

	float normalizedMagnitude = 0;

	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		if (magnitude > 32767)
		{
			magnitude = 32767;
		}

		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	returnValue.x *= normalizedMagnitude;
	returnValue.y *= normalizedMagnitude;

	return returnValue;
}
