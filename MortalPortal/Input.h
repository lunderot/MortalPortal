#pragma once
#include <DirectXMath.h>

class Input
{
protected:
	DirectX::XMFLOAT2 direction;
	
public:
	Input();
	virtual ~Input();

	virtual DirectX::XMFLOAT2 GetDirection(unsigned int playerNr) = 0;
	virtual bool GetButtonState() = 0;
};

