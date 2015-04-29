#pragma once
#include "Button.h"

class StartButton : public Button
{
public:

	StartButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material);
	~StartButton();
	bool isClicked();

};