#pragma once
#include "NumberDisplay.h"
#include "EntityHandler.h"
#include <sstream>
class HighscoreDisplay
{
private:
	unsigned int score[2];
	NumberDisplay* numbersDisplay[2][4];
public:
	HighscoreDisplay(Material* m0, Material* m1, Material* m2, Material* m3, Material* m4,
		Material* m5, Material* m6, Material* m7, Material* m8, Material* m9,
		Geometry* geometry, Shader* shader, EntityHandler* entityHandler);
	~HighscoreDisplay();

	void Update(unsigned int score1, unsigned int score2);
	void SetVisible(bool visible);
};

