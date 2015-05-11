#include "NumberDisplay.h"


NumberDisplay::NumberDisplay(Material* m0, Material* m1, Material* m2, Material* m3, Material* m4,
	Material* m5, Material* m6, Material* m7, Material* m8, Material* m9,
	Geometry* geometry, Shader* shader, unsigned int number)
	: Entity(geometry, m0, shader)
{
	this->numbers[0] = m0;
	this->numbers[1] = m1;
	this->numbers[2] = m2;
	this->numbers[3] = m3;
	this->numbers[4] = m4;
	this->numbers[5] = m5;
	this->numbers[6] = m6;
	this->numbers[7] = m7;
	this->numbers[8] = m8;
	this->numbers[9] = m9;
	if (number < 10)
	{
		this->number = number;
	}
	else
	{
		this->number = 0;
	}
}


NumberDisplay::~NumberDisplay()
{
}


void NumberDisplay::SetNumber(unsigned int number)
{
	if (number < 10)
	{
		this->number = number;
	}
	else
	{
		this->number = 0;
	}
}

unsigned int NumberDisplay::GetNumber() const
{
	return number;
}


Material* NumberDisplay::GetMaterial() const
{
	return numbers[number];
}
