#include "HighscoreDisplay.h"


HighscoreDisplay::HighscoreDisplay(Material* m0, Material* m1, Material* m2, Material* m3, Material* m4,
	Material* m5, Material* m6, Material* m7, Material* m8, Material* m9,
	Geometry* geometry, Shader* shader, EntityHandler* entityHandler)
{
	float offsety = 15.5f;
	float offsetx = 15.0f;

	for (int i = 0; i < 2; i++)
	{
		score[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			numbersDisplay[i][j] = new NumberDisplay(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, geometry, shader, 0);
			numbersDisplay[i][j]->SetPosition(DirectX::XMFLOAT3(offsetx + -j * 2, offsety + offsety * -2 * i, 0));
			entityHandler->Add(numbersDisplay[i][j]);
		}
	}
}


HighscoreDisplay::~HighscoreDisplay()
{
}

void HighscoreDisplay::Update(unsigned int score1, unsigned int score2)
{
	score[0] = score1;
	score[1] = score2;
	
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int y = pow(10, j);
			int z = score[i] / y;
			int x2 = score[i] / (y * 10);
			numbersDisplay[i][j]->SetNumber(z - x2 * 10);
		}
	}
}

void HighscoreDisplay::SetVisible(bool visible)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			numbersDisplay[i][j]->SetVisible(visible);
		}
	}
}
