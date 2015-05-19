#pragma once
#include <DirectXMath.h>

class Input
{
protected:
	DirectX::XMFLOAT2 direction;
	
public:
	Input();
	virtual ~Input();

	virtual DirectX::XMFLOAT2 GetDirection() = 0;
	virtual bool GetButtonState() = 0;
	virtual bool GetButtonEnterState() = 0;
	virtual bool GetButtonUpState() = 0 ;
	virtual bool GetButtonDownState() = 0;
	virtual bool GetButtonStartState() = 0;
};

