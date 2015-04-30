#include "ContinueButton.h"
#include <iostream>
ContinueButton::ContinueButton(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, Material* material)
	: Button(position, scale, material)
{

}

bool ContinueButton::isClicked()
{
	std::cout << "Continue!" << std::endl;
	return false;
}

ContinueButton::~ContinueButton()
{

}