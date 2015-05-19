#pragma once
#include "Digit.h"
#include "EntityHandler.h"

class NumberDisplay
{
private:
	unsigned int number;
	vector<Digit*> numbersDisplay;
	float stridex;
	float scale;
public:
	NumberDisplay(Material* m0, Material* m1, Material* m2, Material* m3, Material* m4,
		Material* m5, Material* m6, Material* m7, Material* m8, Material* m9,
		Geometry* geometry, Shader* shader, EntityHandler* entityHandler, unsigned int numberOfDigits, DirectX::XMFLOAT3 pos);
	~NumberDisplay();

	void Update(unsigned int number);
	void SetVisible(bool visible);

	void SetPosition(DirectX::XMFLOAT3 position);
};

