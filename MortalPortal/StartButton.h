#pragma once
#include "Button.h"

class StartButton : public Button
{
public:

	StartButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Geometry* geometry, Material* material);
	~StartButton();
	void isClicked();

};