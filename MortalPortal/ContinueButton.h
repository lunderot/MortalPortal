#pragma once
#include "Button.h"

class ContinueButton : public Button
{
public:

	ContinueButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material);
	~ContinueButton();

	bool IsClicked();

};