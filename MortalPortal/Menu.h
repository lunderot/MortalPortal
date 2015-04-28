#pragma once
#include "StartButton.h"
#include "Button.h"
#include <d3d11.h>
#include <map>
#include <vector>
#include <string>

class Menu
{
	
public:
	std::vector <Button*> buttons;
	Menu();
	~Menu();

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext);

	void AddButton(Button* button);

};