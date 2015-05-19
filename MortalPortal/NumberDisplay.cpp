#include "NumberDisplay.h"


NumberDisplay::NumberDisplay(Material* m0, Material* m1, Material* m2, Material* m3, Material* m4,
	Material* m5, Material* m6, Material* m7, Material* m8, Material* m9,
	Geometry* geometry, Shader* shader, EntityHandler* entityHandler, unsigned int numberOfDigits, DirectX::XMFLOAT3 pos)
{
	stridex = 0.015f;
	scale = 0.01f;

	for (int i = 0; i < numberOfDigits; i++)
	{
		numbersDisplay.push_back(new Digit(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, geometry, shader, 0));
	}
	int index = 0;
	for (vector<Digit*>::iterator i = numbersDisplay.begin(); i != numbersDisplay.end(); i++)
	{
		(*i)->SetPosition(DirectX::XMFLOAT3(pos.x + -index * stridex, pos.y, pos.z));
		(*i)->SetScale(DirectX::XMFLOAT3(scale, scale * 1.5f, scale));
		entityHandler->Add(*i);
		index++;
	}
}


NumberDisplay::~NumberDisplay()
{
}

void NumberDisplay::Update(unsigned int number)
{
	int index = 0;
	for (vector<Digit*>::iterator i = numbersDisplay.begin(); i != numbersDisplay.end(); i++)
	{
		int y = pow(10, index);
		int z = number / y;
		int x2 = number / (y * 10);
		(*i)->SetNumber(z - x2 * 10);
		index++;
	}
}

void NumberDisplay::SetVisible(bool visible)
{
	for (vector<Digit*>::iterator i = numbersDisplay.begin(); i != numbersDisplay.end(); i++)
	{
		(*i)->SetVisible(visible);
	}
}

void NumberDisplay::SetPosition(DirectX::XMFLOAT3 position)
{
	int index = 0;
	for (vector<Digit*>::iterator i = numbersDisplay.begin(); i != numbersDisplay.end(); i++)
	{
		(*i)->SetPosition(DirectX::XMFLOAT3(position.x + -index * stridex, position.y, position.z));
		index++;
	}
}
