#include "ControllerInput.h"

#include <iostream>
using namespace DirectX;

ControllerInput::ControllerInput(unsigned int playerNr)
{
	/*for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&state, sizeof(state));

		DWORD result = XInputGetState(i, &state);
		if (playerNr == result)
		{
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
		else
		{
			throw std::runtime_error("No controller found");
		}
	}*/
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&state, sizeof(state));
		
		DWORD result = XInputGetState(i, &state);
		if (playerNr == i)
		{
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
}


ControllerInput::~ControllerInput()
{
}

XMFLOAT2 ControllerInput::GetDirection(unsigned int playerNr)
{
	XMFLOAT2 returnValue;

	XInputGetState(id, &state);
	
	if (playerNr == id)
	{
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
	}



	return returnValue;
}

bool ControllerInput::GetButtonState()
{
	XInputGetState(id, &state);
	return state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
}

bool ControllerInput::GetButtonUpState()
{
	XInputGetState(id, &state);
	return state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
}

bool ControllerInput::GetButtonDownState()
{
	XInputGetState(id, &state);
	return state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
}

bool ControllerInput::GetButtonStartState()
{
	XInputGetState(id, &state);
	return state.Gamepad.wButtons & XINPUT_GAMEPAD_START;
}

bool ControllerInput::GetButtonEnterState()
{
	XInputGetState(id, &state);
	return state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
}