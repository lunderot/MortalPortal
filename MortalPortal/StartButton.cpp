#include "StartButton.h"
#include <iostream>
StartButton::StartButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material)
	: Button(position, scale, material)
{

}

void StartButton::isClicked()
{
	std::cout << "START!" << std::endl;
}

StartButton::~StartButton()
{

}