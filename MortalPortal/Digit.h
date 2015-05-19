#pragma once
#include "Entity.h"
#include "Material.h"
#include "Geometry.h"
#include "Shader.h"

class Digit :
	public Entity
{
private:
	Material* numbers[10];
	unsigned int number;
public:
	Digit(Material* m0, Material* m1, Material* m2, Material* m3, Material* m4,
		Material* m5, Material* m6, Material* m7, Material* m8, Material* m9,
		Geometry* geometry, Shader* shader, unsigned int number = 0);
	~Digit();

	void SetNumber(unsigned int number);
	unsigned int GetNumber() const;

	Material* GetMaterial() const;
};

