#pragma once
#include "Button.h"

class OptionsButton : public Button
{
public:

	OptionsButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material);
	~OptionsButton();

	bool isClicked();


};